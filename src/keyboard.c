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
		monitor_write_hex(data);
		monitor_newline();
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

void keyboard_set_autorepeat_delay_and_repeat_rate(unsigned char delay, 
	unsigned char rate)
{
	ASSERT(delay >= 0x1F);
	ASSERT(rate >= 0x03);

	unsigned char data = delay;

	data |= (rate << 5);

	monitor_write("Autorepeat/delay data:");
	monitor_write_bin(data);
	monitor_newline();

	keyboard_enc_send_command(0xF3);
	keyboard_enc_send_command(data);
}

void keyboard_enable()
{
	keyboard_enc_send_command(0xF4);
}

void keyboard_disable()
{
	keyboard_enc_send_command(0xF5);
}

void keyboard_resend_last_byte()
{
	keyboard_enc_send_command(0xFE);
}

bool keyboard_restart()
{
	keyboard_enc_send_command(0xFF);

	switch(keyboard_get_command_return())
	{
	case 0xAA:
		//Self test passed
		return true;
	case 0xFE:
		return keyboard_restart();
	case 0xFC:
	case 0xFD:
		//Self test not passed (TODO:Kanske hantera kanske)
		return false;
	default:
		//Invalid return code
		return false;
	}
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

	monitor_write("Current scan code set:");
	monitor_write_dec(keyboard_get_current_scan_code_set());
	monitor_newline();

	keyboard_set_autorepeat_delay_and_repeat_rate(0x1F, 3);

	register_interrupt_handler(IRQ1, &keyboard_interrupt_handler);

	monitor_writel("Keyboard initialized!");
}