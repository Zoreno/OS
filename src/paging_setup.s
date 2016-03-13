[GLOBAL loadPageDirectory]
[GLOBAL enablePaging]

loadPageDirectory:
	mov		eax, [esp+4]
	mov 	cr3, eax
	ret

enablePaging:

	;Enable protected mode	
	cli
	mov		eax, cr0
	or		eax, 0x00000001
	mov		cr0, eax

	;Enable paging
	mov		eax, cr0
	or		eax, 0x8000000
	mov		cr0, eax
	ret