

SOURCES= loader.o main.o monitor.o io.o io-wrapper.o serial.o common.o gdt.o descriptor_tables.o isr.o interrupt.o keyboard.o kernel_heap.o paging.o paging_setup.o

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector 
LDFLAGS=-Tlink.ld -m elf_i386 -s
ASFLAGS=-felf32

all: $(SOURCES) link

clean:
	-rm *.o kernel.elf

link:
	ld $(LDFLAGS) -o kernel.elf $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<


