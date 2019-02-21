#outb
.section .text
.global outb

outb:
	#instr 	source,dest
	mov	4(%esp), %dx #the adddress where we want to write
	mov 8(%esp), %al  #the data to write to that adddress
	out %al, %dx       #dx = al
	ret
