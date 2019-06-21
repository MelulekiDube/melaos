#include <stddef.h>
#include <stdint.h>
#include "include/io_driver.h"
#include "include/color.h"

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void moveup_createrow(void);

uint8_t make_color(uint8_t fg, uint8_t bg)
{
	return fg | bg << 4;
}

uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}

void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00FF);
}


void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void put_vgaentry(uint16_t entry, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = entry;
	
}

void terminal_newline( void)
{
	terminal_column = 0;
	++terminal_row;
	if(terminal_row == VGA_HEIGHT)
		moveup_createrow();
}

/**
* move all rows up and then discarding the 1st rows
* 
*/
void moveup_createrow(void)
{
	for (uint16_t row = 1; row < VGA_HEIGHT; ++row)
	{
		terminal_row = row-1;
		const size_t index = row * VGA_WIDTH;
		for(uint16_t col = 0; col <VGA_WIDTH; ++col) 
		{
			put_vgaentry(terminal_buffer[index + col], col, terminal_row);
		}
	}	
	++terminal_row;
}
void terminal_putchar(char c)
{
	if(c == '\n')
		terminal_newline();
	else
	{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if ( ++terminal_column == VGA_WIDTH )
		{
			terminal_column = 0;
			if ( ++terminal_row == VGA_HEIGHT )
			{
				moveup_createrow();
			}
		}
	}
	fb_move_cursor((terminal_row*VGA_WIDTH)+terminal_column);
}

void terminal_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
		terminal_putchar(data[i]);
}