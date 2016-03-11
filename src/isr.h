#ifndef ISR_H
#define ISR_H

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct registers
{
	unsigned int ds;

	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;

	unsigned int int_no;
	unsigned int err_code;

	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
	unsigned int useresp;
	unsigned int ss;
} registers_t;

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(unsigned char n, isr_t handler);

#endif