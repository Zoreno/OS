#include "common.h"

#include "monitor.h"

void memset(unsigned char* dest, unsigned char val, unsigned int len)
{
    unsigned char* temp = (unsigned char*)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void memcpy(unsigned char* dest, const unsigned char* src, unsigned int len)
{
    const unsigned char* sp = (const unsigned char*)src;
    unsigned char* dp = (unsigned char*)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
char *strcpy(char *dest, const char *src)
{
    do
    {
      *dest++ = *src++;
    }
    while (*src != 0);
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
char *strcat(char *dest, const char *src)
{
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do
    {
        *dest++ = *src++;
    }
    while (*src != 0);
    return dest;
}

int strlen(char *src)
{
    int i = 0;
    while (*src++)
        i++;
    return i;
}

extern void panic(const char *message, const char *file, unsigned int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    monitor_set_text_color(COLOR_RED);
    monitor_write("PANIC(");
    monitor_write(message);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_newline();
    monitor_set_text_color(COLOR_WHITE);
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, unsigned int line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    monitor_set_text_color(COLOR_RED);
    monitor_write("ASSERTION-FAILED(");
    monitor_write(desc);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_newline();
    monitor_set_text_color(COLOR_WHITE);
    // Halt by going into an infinite loop.
    for(;;);
}

