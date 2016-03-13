#include "monitor.h"

#include "common.h"
#include "descriptor_tables.h"
#include "keyboard.h"
#include "paging.h"

static void key_pressed(scan_code code)
{
	monitor_write("main.c registers keypress:");
	monitor_write_hex(code);
	monitor_newline();
}


/** kernel_main:
 *  The C entry point of the kernel.
 *  
 *	@return 	0 when done
 */
int kernel_main()
{
	monitor_clear();
	monitor_set_text_color(COLOR_GREEN);
	monitor_writel("Kernel boot started...");

	init_descriptor_tables();
	init_keyboard();
	//initialize_paging();

	asm volatile ("sti");

	//unsigned int *ptr = (unsigned int*)0xA0000000;
   	//unsigned int do_page_fault = *ptr;

	monitor_writel("Kernel boot done!");

	register_key_pressed_handler(&key_pressed);

	

	while(1);

	return 0;
}