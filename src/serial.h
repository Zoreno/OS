#ifndef SERIAL_H
#define SERIAL_H

unsigned char serial_read(unsigned short com);
void serial_put(unsigned short com, unsigned char c);
void serial_write(unsigned short com, char* string);

void serial_init_com1();
unsigned char serial_read_com1();
void serial_put_com1(unsigned char c);
void serial_write_com1(char* string);

#endif