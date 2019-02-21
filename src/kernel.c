#include <stddef.h>
#include <stdint.h>
#include "include/io_driver.h"

void kmain()
{
	terminal_initialize();
	terminal_writestring("Hello, to Meluleki's Kernel!\n");
	fb_move_cursor(0);
}
