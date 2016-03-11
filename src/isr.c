#include "isr.h"
#include "monitor.h"
#include "io-wrapper.h"

isr_t interrupt_handlers[256];

void isr_handler(registers_t regs)
{
	if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else
    {
        monitor_write("unhandled interrupt: ");
        monitor_write_dec(regs.int_no);
        monitor_newline();
    }
}

void irq_handler(registers_t regs)
{
	if(regs.int_no >= 40)
	{
		outb_c(0xA0, 0x20);
	}
	outb_c(0x20, 0x20);

	if(interrupt_handlers[regs.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}

void register_interrupt_handler(unsigned char n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}