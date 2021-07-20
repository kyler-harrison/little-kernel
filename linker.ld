OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
	. = 1M;  /* 1 megabyte offset */
	/* does something to define boot.asm */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
	.data : { *(.data) }
	.bss : { *(.bss) }
}
