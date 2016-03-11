#ifndef IO_WRAPPER_H
#define IO_WRAPPER_H

void outb_c(unsigned short port, unsigned char data);
unsigned char inb_c(unsigned short port);

#endif