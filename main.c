/**
 * main.c - Defines the C-code kernel entry point, calls initialisation routines.
 * @version $Id$
 */

#include "monitor.h"

int main(struct multiboot *mboot_ptr)
{
	// Initialise the screen (by clearing it)
	monitor_clear();
	// Write out a sample string
	monitor_write("Hello, World!");

	return 0;
}
