#include "serial.h"
#include "io-wrapper.h"

#define SERIAL_COM1_BASE 				0x3F8 

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

#define SERIAL_LINE_ENABLE_DLAB			0x80

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
	outb_c(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	outb_c(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);
	outb_c(SERIAL_DATA_PORT(com), divisor & 0x00FF);
}

void serial_configure_line(unsigned short com)
{
	//8 bit length, no parity, one stop bit, break control disabled
	outb_c(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_FIFO(unsigned short com)
{
	//Enable FIFO, clear reciever and transmitter, 14 byte queue
	outb_c(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(unsigned short com)
{
	//RTS and DTS enabled
	outb_c(SERIAL_FIFO_COMMAND_PORT(com), 0x03);
}

int serial_recieved(unsigned short com)
{
	return inb_c(SERIAL_LINE_STATUS_PORT(com) & 0x01);
}

unsigned char serial_read(unsigned short com)
{
	while(serial_recieved(com) == 0);

	return inb_c(com);
}

int serial_is_transmit_fifo_empty(unsigned short com)
{
	return inb_c(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_put(unsigned short com, unsigned char c)
{
	while(serial_is_transmit_fifo_empty(com) == 0);

	outb_c(com, c);
}

void serial_write(unsigned short com, char* string)
{
	int i = 0;
	while(string[i])
	{
		serial_put(com, string[i++]);
	}
}

void serial_init_com1()
{
	serial_configure_baud_rate(SERIAL_COM1_BASE, 1);
	serial_configure_line(SERIAL_COM1_BASE);
	serial_configure_FIFO(SERIAL_COM1_BASE);
	serial_configure_modem(SERIAL_COM1_BASE);
}

unsigned char serial_read_com1()
{
	return serial_read(SERIAL_COM1_BASE);
}

void serial_put_com1(unsigned char c)
{
	while(serial_is_transmit_fifo_empty(SERIAL_COM1_BASE) == 0);

	outb_c(SERIAL_COM1_BASE, c);
}

void serial_write_com1(char* string)
{
	int i = 0;
	while(string[i])
	{
		serial_put_com1(string[i++]);
	}
}