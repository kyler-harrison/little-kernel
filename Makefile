CP := cp
RM := rm -rf
MKDIR := mkdir -pv

BIN = kernel
CFG = grub.cfg
ISO_PATH := iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub
ISO_NAME = little-kernel

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
	$(MKDIR) $(GRUB_PATH)
	$(CP) $(BIN)/$(BIN) $(BOOT_PATH)
	$(CP) $(CFG) $(GRUB_PATH)
	grub-file --is-x86-multiboot $(BOOT_PATH)/$(BIN)
	grub-mkrescue -o $(ISO_NAME).iso $(ISO_PATH)

PHONY: clean
clean:
		$(RM) kernel/*.o kernel/kernel
		$(RM) boot/*.o
		$(RM) *iso/
