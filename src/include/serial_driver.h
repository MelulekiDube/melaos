#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H
/*
	@brief The I/O ports.
	Below is the Definition for the COM port base values.
	These are COM1, COM2, COM3, and COM4
*/
#define SERIAL_COM1_BASE 0x3F8 /* COM1 base port */
#define SERIAL_COM2_BASE 0x2E8 /*COM2 base port*/
#define SERIAL_COM3_BASE 0x3E8 /*COM3 base port*/
#define SERIAL_COM4_BASE 0x2E8 /*COM4 base port*/

/*
P   D         register mapped
--------------------------------------------------------------------------------------------------------------
+0 0  Data register. Reading this registers read from the Receive buffer. 
	  Writing to this register writes 	to the Transmit buffer.
--------------------------------------------------------------------------------------------------------------
+1	0	Interrupt Enable Register.
--------------------------------------------------------------------------------------------------------------
+0	1	With DLAB set to 1, this is the least significant byte of the divisor value for setting the baud rate.
--------------------------------------------------------------------------------------------------------------
+1	1	With DLAB set to 1, this is the most significant byte of the divisor value.
--------------------------------------------------------------------------------------------------------------
+2	-	Interrupt Identification and FIFO control registers
--------------------------------------------------------------------------------------------------------------
+3	-	Line Control Register. The most significant bit of this register is the DLAB.
--------------------------------------------------------------------------------------------------------------
+4	-	Modem Control Register.
--------------------------------------------------------------------------------------------------------------
+5	-	Line Status Register.
--------------------------------------------------------------------------------------------------------------
+6	-	Modem Status Register.
--------------------------------------------------------------------------------------------------------------
+7	-	Scratch Register.
--------------------------------------------------------------------------------------------------------------

*/
#define SERIAL_DATA_PORT(base) (base)  //data register
#define SERIAL_INTERUPT_ENABLE_PORT(base) (base+1)
#define SERIAL_FIFO_COMMAND_PORT(base) (base + 2) //Interrupt Identification and FIFO control registers
#define SERIAL_LINE_COMMAND_PORT(base) (base + 3) //Line control register
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4) //Modem control register
#define SERIAL_LINE_STATUS_PORT(base) (base + 5)   //Line status register
#define SERIAL_MODEM_STATUS_PORT(base) (base+6)   //Modem status register

/* SERIAL_LINE_ENABLE_DLAB:
* Tells the serial port to expect first the highest 8 bits on the data port,
* then the lowest 8 bits will follow
*/
#define SERIAL_LINE_ENABLE_DLAB 0x80 //this is equivalent to 1000000 therefore we are setting one to the msb of the DLAB thereby enabling it.
#define DISABLER 0X00
#define MASK_CLEAR_16_MSB  0X00FF //this A mask that is used to clear the 16 msbs of any number.
#define LINE_8N1_PROTOCOL 0x03 //this is 00000011 that last two bits are for 8bits, the 2nd bit is for 1 stop sign and the third bit is for no parity. the rest don't matter
#define FIFO_CONFIG 0XC7
#define DEFAULT_BAUD_DIVISOR 0X03
/** serial_configure_baud_rate:
* Sets the speed of the data being sent. The default speed of a serial
* port is 115200 bits/s. The argument is a divisor of that number, hence
* the resulting speed becomes (115200 / divisor) bits/s.
27
*
* @param com The COM port to configure
* @param divisor The divisor 
*/
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);//short is 16bits

/** serial_configure_line:
* Configures the line of the given serial port. The port is set to have a
* data length of 8 bits, no parity bits, one stop bit and break control
* disabled.
*
* @param com The serial port to configure
*/
void serial_configure_line(unsigned short com);


/**serial_init_port:
* will configure the port by calling configure_baud_rate and configure_line methods. 
* It will also disable interrupts, Enable the FIFO, clear them, with 14-byte threshold
* It will also enable IRQs
* @param com the serial port to configure
* @param the divisor for setting the baud rate of this port
*/
void serial_init_port(unsigned short com, unsigned short divisor);

/**serial_received:
* This functions checks port line status to see if there is data ready.
* @param com the port we are reading data from
* @returns an int value which signals if there is data or not.
*/
int serial_received(unsigned short com);//checks to see if the port line_status has new info

/**read_serial:
* This functions reads data on the particular port.
* @param com the port we are reading data from
* @returns 8bits of read data.
*/
char read_serial(unsigned short com); //polls until serial_received returns a 1


/**is_transmit_empty:
* @brief Checks whether the transmit FIFO queue is empty or not for the given COM
* port
* @param the port that we want to investigate the transmit line for.
* @return 0 if the transmit FIFO queue is not empty
* 		  1 if the transmit FIFO queue is empty
*/
int is_transmit_empty(unsigned short com); 

/**write_serial:
* @brief This method will write the character given to the data register of the port given as com
* @param com the port we are writing to
* @param a the character we are writing
*/
void write_serial(unsigned short com, char a);