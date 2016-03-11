[GLOBAL outb]
[GLOBAL inb]

outb:
	mov al, [esp + 8]		;Load data from stack
	mov dx, [esp + 4] 		;Load port from stack
	out dx, al				;Write data to port
	ret

inb:
	mov dx, [esp + 4]		;I/O port address 
	in 	al, dx				;Read byte and store in al
	ret