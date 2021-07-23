bits 32

; defines multiboot specification
section .multiboot           
	; dd is define double, 4 bytes on 32-bit x86 machine
	dd 0x1BADB002            ; "magic number" for bootloader 
	dd 0x0	                 ; set flags (none i think?)
	dd - (0x1BADB002 + 0x0)  ; checksum

; where the actual assembly code goes
section .text
global start                 ; where execution begins (see below)
extern main                  ; main function defined in C file

start:
	cli                      ; clear interrupts
	mov esp, stack_space     ; copy empty stack function into stack pointer 
	call main                ; runs the main from C file
    hlt                      ; halts cpu execution

; where variables are declared
section .bss
resb 8192                    ; 8 kb for stack_space
stack_space:

