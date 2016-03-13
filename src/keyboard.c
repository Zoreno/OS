#include "keyboard.h"
#include "io-wrapper.h"
#include "isr.h"
#include "common.h"
#include "monitor.h"

#define KBD_DATA_PORT 		0x60
#define KBD_COMMAND_PORT 	0x64

typedef unsigned char scan_code;

unsigned char read_data_port()
{
	return inb_c(KBD_DATA_PORT);
}

scan_code read_scan_code()
{
	return inb_c(KBD_DATA_PORT);
}

unsigned char read_keyboard_ctrl_status()
{
	return inb_c(KBD_COMMAND_PORT);
}

unsigned char keyboard_get_command_return()
{ 
	unsigned int data;

	while(true)
	{
		while(true)
		{
			if((read_keyboard_ctrl_status() & 0x01) != 0)
				break;
		}
	
		data = read_data_port();
		if(data != 0xFA) break; 

	}
	return data;
}

void keyboard_ctrl_send_command(unsigned char com)
{
	while(true)
	{
		if((read_keyboard_ctrl_status() & 0x02) == 0)
			break;

	}
	outb_c(KBD_COMMAND_PORT, com);
}

void keyboard_enc_send_command(unsigned char com)
{
	while(true)
	{
		if((read_keyboard_ctrl_status() & 0x02) == 0)
			break;

	}
	outb_c(KBD_DATA_PORT, com);
}

void keyboard_set_LED(bool caps, bool scroll, bool num)
{
	unsigned char data = 0;

	if(caps) 
	{
		data |= 0x01;
	} 

	if(scroll) 
	{
		data |= 0x02;
	} 

	if(num) 
	{
		data |= 0x04;
	} 

	ASSERT((data & 0xF8) == 0);

	keyboard_enc_send_command(0xED);
	keyboard_enc_send_command(data);
}

void keyboard_set_alternate_scan_code_set(unsigned char set)
{
	unsigned char data = 0;
	switch(set)
	{
	case 1:
		data = 0x01;
		break;
	case 2:
		data = 0x02;
		break;
	case 3:
		data = 0x04;
		break;
	default:
		PANIC("Invalid scan code set, should be 1,2 or 3");
	}
	keyboard_enc_send_command(0xF0);
	keyboard_enc_send_command(data);
}

unsigned char keyboard_get_current_scan_code_set()
{
	keyboard_enc_send_command(0xF0);
	keyboard_enc_send_command(0x00);

	return keyboard_get_command_return();
}

static void keyboard_interrupt_handler(registers_t regs)
{
	scan_code code  = read_data_port();
	if(code == 0xFA) return;
	monitor_write("Keyboard interrupt! Scancode: ");
	monitor_write_hex(code);
	monitor_newline();
}

void init_keyboard()
{
	monitor_writel("Starting to initialize keyboard...");

	keyboard_set_LED(true,true,true);

	monitor_write_hex(keyboard_get_current_scan_code_set());
	monitor_newline();

	register_interrupt_handler(IRQ1, &keyboard_interrupt_handler);

	monitor_writel("Keyboard initialized!");
}