#include "kernel_heap.h"

extern unsigned int end;
unsigned int placement_address = (unsigned int)&end;

unsigned int kmalloc_int(unsigned int size, int align, unsigned int *phys)
{
	if(align == 1 && (placement_address & 0xFFFFF000))
	{
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if(phys)
	{
		*phys = placement_address;
	}
	unsigned int tmp = placement_address;
	placement_address += size;
	return tmp;
}

unsigned int kmalloc_a(unsigned int size)
{
	return kmalloc_int(size, 1, 0);
}
unsigned int kmalloc_p(unsigned int size, unsigned int *phys)
{
	return kmalloc_int(size, 0, phys);
}
unsigned int kmalloc_ap(unsigned int size, unsigned int *phys)
{
	return kmalloc_int(size, 1, phys);
}
unsigned int kmalloc(unsigned int size)
{
	return kmalloc_int(size, 0, 0);
}