.PHONY: all
all: bootloader kernel.obj linker iso
	@echo Make complete.

bootloader: boot/boot.asm
	nasm -f elf32 boot/boot.asm -o boot/boot.o

kernel.obj: kernel/kernel.c drivers/display.c drivers/keyboard.c
	gcc -m32 -c kernel/kernel.c -o kernel/kernel.o 
	gcc -m32 -c drivers/display.c -o drivers/display.o 
	gcc -m32 -c drivers/keyboard.c -o drivers/keyboard.o 

linker: linker.ld boot/boot.o kernel/kernel.o drivers/display.o drivers/keyboard.o
	ld -m elf_i386 -T linker.ld -o kernel/kernel boot/boot.o drivers/display.o drivers/keyboard.o kernel/kernel.o

iso: kernel.obj
	mkdir -pv iso/boot/grub
	cp kernel/kernel iso/boot
	cp grub.cfg iso/boot/grub
	grub-file --is-x86-multiboot iso/boot/kernel
	grub-mkrescue -o little-kernel.iso iso

.PHONY: clean
clean:
		rm -rf kernel/*.o kernel/kernel
		rm -rf boot/*.o
		rm -rf drivers/*.o
		rm -rf *iso/
