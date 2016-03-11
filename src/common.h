#ifndef COMMON_H
#define COMMON_H

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

void memset(unsigned char* dest, unsigned char val, unsigned int len);
void memcpy(unsigned char* dest, const unsigned char* src, unsigned int len);

extern void panic(const char* message, const char* file, unsigned int line);
extern void panic_assert(const char* file, unsigned int line, const char *desc);

#endif
