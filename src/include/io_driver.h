#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include <stdint.h>
/** outb:
* Sends the given data to the given I/O port. Defined in io.s
*
* @param port The I/O port to send the data to
* @param data The data to send to the I/O port
*/
extern void outb(unsigned short port, unsigned char data);
extern int inb(unsigned short com);

uint8_t make_color(uint8_t fg, uint8_t bg);
uint8_t make_color(uint8_t fg, uint8_t bg);
uint16_t make_vgaentry(char c, uint8_t color);
size_t strlen(const char* str);
void terminal_initialize();
void terminal_setcolor(uint8_t color);
void fb_move_cursor(unsigned short pos);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void put_vgaentry(uint16_t entry, size_t x, size_t y);
void terminal_newline( void);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void moveup_createrow(void);
#endif /* INCLUDE_IO_H */
