/**
 * main.c - Defines the C-code kernel entry point, calls initialisation routines.
 * @version $Id$
 */

#include "monitor.h"
#include "descriptor_tables.h"

int main(struct multiboot *mboot_ptr)
{
	// Initialise all the ISRs and segmentation
	init_descriptor_tables();
	// Initialise the screen (by clearing it)
	monitor_clear();
	// Write out a sample string
	monitor_write("Hello, World!\n");

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	return 0;
}
