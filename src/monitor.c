#include "monitor.h"
#include "io-wrapper.h"

//Pointer to video memory (frame buffer)
char* fb = (char*) 0x000B8000; 

//Hardware cursor position
unsigned short  cursor_x = 0;	
unsigned short  cursor_y = 0;

//Color behind text
MONITOR_COLOR back_color = COLOR_BLACK;
//Color of text
MONITOR_COLOR text_color = COLOR_WHITE;

//Definitions for ports related to cursor movement
#define FB_COMMAND_PORT			0x3D4
#define FB_DATA_PORT			0x3D5
#define FB_HIGH_BYTE_COMMAND	14
#define FB_LOW_BYTE_COMMAND		15

/** scroll:
 *  Scrolls the terminal one row.
 */
void scroll()
{
	unsigned char attributeByte = ((COLOR_BLACK & 0x0F) << 4) | (COLOR_WHITE & 0x0F);
	unsigned short  blank = 0x20 | (attributeByte << 8);

	if(cursor_y >= MONITOR_HEIGHT)
	{
		//Move all characters one row up
		int i;
		for(i = 0; i < (MONITOR_HEIGHT-1)*MONITOR_WIDTH; ++i)
		{
			fb[2*i] = fb[2*(i + MONITOR_WIDTH)];
		}
		//Insert blank row at bottom
		for(i = (MONITOR_HEIGHT-1)*MONITOR_WIDTH; i < MONITOR_HEIGHT*MONITOR_WIDTH; ++i)
		{
			fb[2*i] = blank;
		}
		cursor_y = (MONITOR_HEIGHT-1);
	}
}

/** move_cursor:
 *  Move the hardware cursor to (cursor_x, cursor_y)
 */
void move_cursor()
{
	unsigned short cursor_location = cursor_y * MONITOR_WIDTH + cursor_x;

	outb_c(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb_c(FB_DATA_PORT, ((cursor_location) >> 8) & 0x00FF);
	outb_c(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb_c(FB_DATA_PORT, (cursor_location) & 0x00FF);

}

/** monitor_put:
 *  Prints a character at (cursor_x, cursor_y) and moves the cursor to the 
 *  next position.
 *
 *	@param 	c 		The character to be printed
 */
void monitor_put(const char c)
{
	unsigned short cursor_location = cursor_y * MONITOR_WIDTH + cursor_x;
	fb[(2*cursor_location)] = c;
	fb[(2*cursor_location) + 1] = ((back_color & 0x0F) << 4) | (text_color & 0x0F);
	if(++cursor_x>=MONITOR_WIDTH)
	{
		cursor_x=0;
		cursor_y++;
	}

	scroll();

	move_cursor();
}

/** monitor_write:
 *  Prints a string of characters starting at (cursor_x, cursor_y) and moves 
 *  the cursor to the next position.
 *
 *	@param 	string 		The string to be printed
 */
void monitor_write(const char* string)
{
	int i = 0;
	while(string[i])
	{
		monitor_put(string[i++]);
	}
}

/** monitor_writel:
 *  Prints a string of characters starting at (cursor_x, cursor_y) and moves 
 *  the cursor to the next line.
 *
 *	@param 	string 		The string to be printed
 */
void monitor_writel(const char* string)
{
	monitor_write(string);
	monitor_newline();
}

/** monitor_newline:
 *  Moves the cursor to the start of the next line.
 */
void monitor_newline()
{
	cursor_x = 0;
	cursor_y++;

	scroll();

	move_cursor();
}

/** monitor_clear:
 *  Clears the entire screen.
 */
void monitor_clear()
{
	unsigned char attributeByte = ((COLOR_BLACK & 0x0F) << 4) | (COLOR_WHITE & 0x0F);
	unsigned short blank = 0x20;

	int i;
	for(i = 0; i < MONITOR_WIDTH*MONITOR_HEIGHT; ++i)
	{
		fb[2*i] = blank;
		fb[2*i + 1] = attributeByte;
	}

	cursor_x = 0;
	cursor_y = 0;

	move_cursor();
}

/** print_num:
 *  Prints a number in a given base.
 *  
 *	@param 	n 		Number to be printed
 *	@param 	base 	Base to be used.
 */
void print_num(const unsigned int n,const unsigned char base)
{
	unsigned char last_digit = n % base;

	if(n >= base)
	{
		print_num(n/base, base);
	} 

	//Correct for ascii characters that are not digits
	if(last_digit > 9)
	{
		last_digit += 7;
	}
	monitor_put('0' + last_digit);
}

/** monitor_write_dec:
 *  Prints a number in decimal.
 *  
 *	@param 	n 		Number to be printed
 */
void monitor_write_dec(const unsigned int n)
{
	print_num(n, 10);
}

/** monitor_write_hex:
 *  Prints a number in hexadecimal.
 *  
 *	@param 	n 		Number to be printed
 */
void monitor_write_hex(const unsigned int n)
{
	monitor_write("0x");
	print_num(n, 16);
}

/** monitor_write_oct:
 *  Prints a number in octal.
 *  
 *	@param 	n 		Number to be printed
 */
void monitor_write_oct(const unsigned int n)
{
	monitor_write("0");
	print_num(n, 8);
}

/** monitor_write_bin:
 *  Prints a number in binary.
 *  
 *	@param 	n 		Number to be printed
 */
void monitor_write_bin(const unsigned int n)
{
	monitor_write("0b");
	print_num(n, 2);
}

/** monitor_set_text_color:
 *  Changes the text color for following character prints
 *  
 *	@param 	color 	The new color
 */
void monitor_set_text_color(MONITOR_COLOR color)
{
	text_color = color;
}

/** monitor_set_back_color:
 *  Changes the background color for following character prints
 *  
 *	@param 	color 	The new color
 */
void monitor_set_back_color(MONITOR_COLOR color)
{
	back_color = color;
}