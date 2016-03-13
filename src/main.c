#include "monitor.h"

#include "common.h"
#include "descriptor_tables.h"
#include "keyboard.h"

static void key_pressed(scan_code code)
{
	monitor_write("main.c registers Keypress:");
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
	monitor_set_back_color(COLOR_BLUE);
	monitor_set_text_color(COLOR_WHITE);
	monitor_clear();
	monitor_writel("Kernel boot started...");

	init_descriptor_tables();
	init_keyboard();

	asm volatile ("sti");

	monitor_writel("Kernel boot done!");

	register_key_pressed_handler(&key_pressed);

	while(1);

	return 0;
}
