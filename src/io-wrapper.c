#include "io-wrapper.h"

extern void outb(unsigned short port, unsigned char data);
extern unsigned char inb(unsigned short port);

void outb_c(unsigned short port, unsigned char data)
{
	outb(port, data);
}

unsigned char inb_c(unsigned short port)
{
	inb(port);
}