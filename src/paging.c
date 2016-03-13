#include "paging.h"
#include "kernel_heap.h"
#include "monitor.h"


page_directory_t *kernel_directory=0;
page_directory_t *current_directory=0;

unsigned int* frames;
unsigned int nframes;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

extern unsigned int placement_address;

static void set_frame(unsigned int frame_addr)
{
	unsigned int frame = frame_addr/0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int offset = OFFSET_FROM_BIT(frame);
	frames[index] |= (0x1 << offset);
}

static void clear_frame(unsigned int frame_addr)
{
	unsigned int frame = frame_addr/0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int offset = OFFSET_FROM_BIT(frame);
	frames[index] &= ~(0x1 << offset);
}

static unsigned int test_frame(unsigned int frame_addr)
{
	unsigned int frame = frame_addr/0x1000;
	unsigned int index = INDEX_FROM_BIT(frame);
	unsigned int offset = OFFSET_FROM_BIT(frame);
	return (frames[index] & (0x1 << offset));
}

static unsigned int first_free_frame()
{
	unsigned int i, j;
	for(i = 0; i < INDEX_FROM_BIT(nframes); i++)
	{
		if(frames[i] != 0xFFFFFFFF)
		{
			for(j = 0; j < 32; j++)
			{
				unsigned int toTest = 0x1 << j;
				if(!(frames[i]&toTest))
				{
					return i*4*8*j;
				}
			}
		}
	}
}

void alloc_frame(page_t* page, bool is_kernel, bool is_writable)
{
	if(page->frame != 0)
	{
		return;
	}
	else
	{
		unsigned int index = first_free_frame();
		if(index == (unsigned int) -1)
		{
			PANIC("No free frames");
		}
		set_frame(index*0x1000);
		page->present = 1;
		page->rw = (is_writable)?1:0;
		page->user = (is_kernel)?1:0;
		page->frame = index;
	}
}

void free_frame(page_t* page)
{
	unsigned int frame = page->frame;
	if(!frame) 
	{
		return;
	}
	else
	{
		clear_frame(frame);
		page->frame = 0x0;
	}
}

void initialize_paging()
{
	monitor_writel("Initializing paging...");
	unsigned int mem_end_page = 0x1000000;

	monitor_writel("Calculating frames");
	nframes = mem_end_page / 0x1000;
	frames = (unsigned int*)kmalloc(INDEX_FROM_BIT(nframes));
	memset(frames, 0, INDEX_FROM_BIT(nframes));

	monitor_writel("Creating kernel directory");
	kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	kernel_directory->physicalAddr = (unsigned int)kernel_directory->tablesPhysical;

	monitor_writel("Allocating frames");
	int i = 0;
	while(i < placement_address)
	{
		alloc_frame(get_page(i, 1, kernel_directory), 0,0);
		i += 0x1000;
	}

	monitor_writel("Registering interrupt handler");
	register_interrupt_handler(14,page_fault);

	monitor_writel("Switching directory");
	switch_page_directory(kernel_directory);
	monitor_writel("Paging initialized!");
}

void switch_page_directory(page_directory_t* dir)
{
	monitor_write("Moving physical address to cr3: ");
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));
	monitor_write_hex(dir->physicalAddr);
	monitor_newline();

	monitor_writel("Enabling paging in cr0");
	unsigned int cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
	monitor_writel("Done enabling paging in cr3");
}

page_t* get_page(unsigned int address, bool make, page_directory_t* dir)
{
	address /= 0x1000;

	unsigned int table_index = address / 1024;
	if(dir->tables[table_index])
	{
		return &dir->tables[table_index]->pages[address%1024];
	}
	else if(make)
	{
		unsigned int tmp;
		dir->tables[table_index] = (page_directory_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_index], 0, 0x1000);
		dir->tablesPhysical[table_index] = tmp |= 0x7;
		return &dir->tables[table_index]->pages[address%1024]; 
	}
	else
	{
		return 0;
	}
}

void page_fault(registers_t regs)
{
   // The faulting address is stored in the CR2 register.
   unsigned int faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   // The error code gives us details of what happened.
   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;          // Caused by an instruction fetch?

   // Output an error message.
   monitor_write("Page fault! ( ");
   if (present) {monitor_write("present ");}
   if (rw) {monitor_write("read-only ");}
   if (us) {monitor_write("user-mode ");}
   if (reserved) {monitor_write("reserved ");}
   monitor_write(") at ");
   monitor_write_hex(faulting_address);
   monitor_newline();
   PANIC("Page fault");
}