; fact.asm

SECTION .text ; program
global addition ; linux
global _addition ; windows
addition:
_addition:
	mov eax, [esp+4]
	add eax, [esp+8]
	ret