AS:=i686-elf-as
CC:=i686-elf-gcc
XCC :=gcc
XASM :=nasm

NFLAGS :=  -f win32
CFLAGS:=-ffreestanding -O2 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs
CPPFLAGS:=
LIBS:=-lgcc

OBDIR:=objects
BINDIR:=bin
SPDIR := spike
OBJS:=\
$(OBDIR)/boot.o \
$(OBDIR)/io.o\
$(OBDIR)/io_driver.o\
$(OBDIR)/kernel.o \


ifeq ($(OS),Windows_NT)
	MK:=C:\cygwin64\bin\mkdir
else 
	MK:=mkdir
endif

all:$(BINDIR)/melaos.bin

.PHONEY: all clean iso run-qemu

$(BINDIR)/melaos.bin: $(OBJS)
	$(MK) -p bin
	$(CC) -T src/linker.ld -o $@ $(CFLAGS) $(OBJS) $(LIBS)
	
$(OBDIR)/%.o: src/%.s
	$(MK) -p $(OBDIR)
	$(AS) $< -o $@
	
$(OBDIR)/%.o: src/%.c
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

spike: 
	$(XASM) -o $(SPDIR)/add.o $(SPDIR)/add.asm
	$(XCC) -o $(SPDIR)/add $(SPDIR)/add.c $(SPDIR)/add.o

srun:
	$(SPDIR)/add

clean:
	rm -f -r isodir
	rm -f melaos.bin melaos.iso
	rm -f -r $(BINDIR) $(OBDIR)

iso: melaos.iso

isodir isodir/boot isodir/boot/grub:
	$(MK) -p $@

isodir/boot/melaos.bin: $(BINDIR)/melaos.bin isodir/boot
	cp $< $@

isodir/boot/grub/grub.cfg: grub/grub.cfg isodir/boot/grub
	cp $< $@

melaos.iso: isodir/boot/melaos.bin isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

run: melaos.iso
	qemu-system-i386 -cdrom melaos.iso