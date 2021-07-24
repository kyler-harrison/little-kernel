.PHONY: all
all: bootloader kernel.obj linker iso
	@echo Make complete.

bootloader: boot/boot.asm
	nasm -f elf32 boot/boot.asm -o boot/boot.o

kernel.obj: kernel/kernel.c
	gcc -m32 -c kernel/kernel.c -o kernel/kernel.o

linker: linker.ld boot/boot.o kernel/kernel.o
	ld -m elf_i386 -T linker.ld -o kernel/kernel boot/boot.o kernel/kernel.o

iso: kernel.obj
	mkdir -pv iso/boot/grub
	cp kernel/kernel iso/boot
	cp grub.cfg iso/boot/grub
	grub-file --is-x86-multiboot iso/boot/kernel
	grub-mkrescue -o little-kernel.iso iso

PHONY: clean
clean:
		rm -rf kernel/*.o kernel/kernel
		rm -rf boot/*.o
		rm -rf *iso/
