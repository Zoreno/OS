#include "monitor.h"

#include "common.h"
#include "descriptor_tables.h"

/** kernel_main:
 *  The C entry point of the kernel.
 *  
 *	@return 	0 when done
 */
int kernel_main()
{
	monitor_clear();
	monitor_writel("Kernel boot started...");

	init_descriptor_tables();
	init_keyboard();

	asm volatile ("sti");

	monitor_writel("Kernel boot done!");

	while(1);

	return 0;
}