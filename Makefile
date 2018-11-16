AS:=i686-elf-as
CC:=i686-elf-gcc

CFLAGS:=-ffreestanding -O2 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs
CPPFLAGS:=
LIBS:=-lgcc

OBJS:=\
boot.o \
kernel.o \

all: melaos.bin

.PHONEY: all clean iso run-qemu

melaos.bin: $(OBJS)
	$(CC) -T src/linker.ld -o $@ $(CFLAGS) $(OBJS) $(LIBS)

%.o: src/%.c
	$(CC) -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

%.o: src/%.s
	$(AS) $< -o $@

clean:
	rm -rf isodir
	rm -f melaos.bin melaos.iso $(OBJS)

iso: melaos.iso

isodir isodir/boot isodir/boot/grub:
	mkdir -p $@

isodir/boot/melaos.bin: melaos.bin isodir/boot
	cp $< $@

isodir/boot/grub/grub.cfg: grub.cfg isodir/boot/grub
	cp $< $@

melaos.iso: isodir/boot/melaos.bin isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ isodir

run: melaos.iso
	qemu-system-i386 -cdrom melaos.iso