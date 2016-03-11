#include "keyboard.h"
#include "io-wrapper.h"
#include "isr.h"
#include "monitor.h"

#define KBD_DATA_PORT 		0x60

unsigned char read_scan_code()
{
	return inb_c(KBD_DATA_PORT);
}

static void keyboard_interrupt_handler(registers_t regs)
{
	monitor_write("Keyboard interrupt! Scancode: ");
	monitor_write_dec(read_scan_code());
	monitor_newline();
}

void init_keyboard()
{
	monitor_writel("Starting to initialize keyboard...");
	register_interrupt_handler(IRQ1, &keyboard_interrupt_handler);
	monitor_writel("Keyboard initialized!");
}