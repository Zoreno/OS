[GLOBAL loader]							;ELF entry point

MAGIC_NUMBER 	equ 0x1BADB002 			;Magic number constant
FLAGS			equ	0x1 				;Multiboot flags
CHECKSUM		equ -(MAGIC_NUMBER+FLAGS) 		;Checksum for ELF

KERNEL_STACK_SIZE equ 4096				;Maximum stack size

[EXTERN kernel_main]	
[EXTERN end]				

section .text							;Start of code
align 4 								;4 byte aligned
	dd MAGIC_NUMBER 					;Write constants to machine
	dd FLAGS
	dd CHECKSUM
  	dd end                       		; End of kernel.

loader:									;Entry point
	mov esp, kernel_stack + KERNEL_STACK_SIZE 	;Kernel stack start point

	call kernel_main 					;Call kernel main functin (defined in main.c)

	cli									;Clear interrupt
	hlt									;Halt processor
	jmp $								;Infinite loop

section .bss							;Start of bss section
align 4 								;4 byte align
kernel_stack:							
	resb KERNEL_STACK_SIZE				;Reserve uninitialized storage space
	
