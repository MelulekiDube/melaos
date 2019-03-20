#include "include/port_config.h"
#include "include/serial_driver.h"

/** serial_configure_baud_rate:
* Sets the speed of the data being sent. The default speed of a serial
* port is 115200 bits/s. The argument is a divisor of that number, hence
* the resulting speed becomes (115200 / divisor) bits/s.
27
*
* @param com The COM port to configure
* @param divisor The divisor 
*/
void serial_configure_baud_rate(unsigned short com, unsigned short divisor){
	outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB); //enable dlab might wanna change so we set and not write
	//we send the lsb to port+0
	outb(SERIAL_DATA_PORT, (divisor&MASK_CLEAR_16_MSB));
	//we send the msb to port+1
	outb(SERIAL_INTERUPT_ENABLE_PORT(com), (divisor>>8)&MASK_CLEAR_16_MSB);
	outb(SERIAL_LINE_COMMAND_PORT(com), DISABLER); //disable dlab by clearing the msb
}

/** serial_configure_line:
* Configures the line of the given serial port. The port is set to have a
* data length of 8 bits, no parity bits, one stop bit and break control
* disabled.
*
* @param com The serial port to configure
*/
void serial_configure_line(unsigned short com){
	/* Bit: 	| 7 | 6 | 5 4 3 | 2 | 1 0 |
	* Content:  | d | b | prty  | s | dl |
	* Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
	*/
	outb(SERIAL_LINE_COMMAND_PORT(com), LINE_8N1_PROTOCOL);
}


/**serial_init_port:
* will configure the port by calling configure_baud_rate and configure_line methods. 
* It will also disable interrupts, Enable the FIFO, clear them, with 14-byte threshold
* It will also enable IRQs
* @param com the serial port to configure
* @param the divisor for setting the baud rate of this port
*/
void serial_init_port(unsigned short com, unsigned short divisor=DEFAULT_BAUD_DIVISOR){
	outb(SERIAL_INTERUPT_ENABLE_PORT(com), DISABLER); //Disable all interrupts
	serial_configure_baud_rate(com, divisor);
	serial_configure_line(com);
	outb(SERIAL_FIFO_COMMAND_PORT(com), FIFO_CONFIG);
	outb(SERIAL_MODEM_COMMAND_PORT(com), 0x0B);  // IRQs enabled, RTS/DSR set
}

/**serial_received:
* This functions checks port line status to see if there is data ready.
* @param com the port we are reading data from
* @returns an int value which signals if there is data or not.
*/
int serial_received(unsigned short com) {
   return inb(SERIAL_LINE_STATUS_PORT(com)) & 1;
}
 
/**read_serial:
* This functions reads data on the particular port.
* @param com the port we are reading data from
* @returns 8bits of read data.
*/
char read_serial(unsigned short com) {
   while (!serial_received(com));
 
   return inb(PORT);
}

//sending data to port
int is_transmit_empty(unsigned short com) {
	/* 0x20 = 0010 0000 since the bit 5 is responsible for that*/
   return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}
 
 

void write_serial(unsigned short com, char a) {
   while (is_transmit_empty() == 0);
 
   outb(SERIAL_DATA_PORT(com),a);
}
