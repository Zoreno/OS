#ifndef MONITOR_H
#define MONITOR_H

#define MONITOR_WIDTH 80
#define MONITOR_HEIGHT 25

//enum for existing monitor colors
typedef enum   {COLOR_BLACK = 0,
				COLOR_BLUE = 1,
				COLOR_GREEN = 2,
				COLOR_CYAN = 3,
				COLOR_RED = 4,
				COLOR_MAGENTA = 5,
				COLOR_BROWN = 6,
				COLOR_LIGHT_GREY = 7,
				COLOR_DARK_GREY = 8,
				COLOR_LIGHT_BLUE = 9,
				COLOR_LIGHT_GREEN = 10,
				COLOR_LIGHT_CYAN = 11,
				COLOR_LIGHT_RED = 12,
				COLOR_LIGHT_MAGENTA = 13,
				COLOR_LIGHT_BROWN = 14,
				COLOR_WHITE = 15}
	MONITOR_COLOR;

//Functions to put characters on the screen
void monitor_put(const char c);
void monitor_write(const char* string);
void monitor_writel(const char* string);

//Misc. functions for screen control
void monitor_newline();
void monitor_clear();

//Functions to put numbers on the screen
void monitor_write_dec(const unsigned int n);
void monitor_write_hex(const unsigned int n);
void monitor_write_oct(const unsigned int n);
void monitor_write_bin(const unsigned int n);

//Functions for changing color
void monitor_set_text_color(MONITOR_COLOR color);
void monitor_set_back_color(MONITOR_COLOR color);

#endif