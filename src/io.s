#outb and inb
.section .text
.global outb
.global inb
outb:
	#instr 	source,dest
	mov	4(%esp), %dx #the adddress where we want to write
	mov 8(%esp), %al  #the data to write to that adddress
	out %al, %dx       #dx = al
	ret

# inb - returns a byte from the given I/O port
# stack: [esp + 4] The address of the I/O port
# [esp ] The return address
inb:
	mov 4(%esp), %dx  #esp4 has the address we want to write to. theerfore we re saying dx = that adddress 
	in %dx, %al      #%al now has the contents from the adddress we where reading from.
	ret
	 