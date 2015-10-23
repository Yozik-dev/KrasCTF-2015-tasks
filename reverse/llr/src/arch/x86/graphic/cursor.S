	.code16gcc
	.text
	.intel_syntax noprefix

	.global arch_move_cursor, arch_get_cursor_pos
	/**
	 * Set cursor into (x, y)
	 * This function accepts two integers
	 * x - is for X coordinate
	 * y - is for Y coordinate
	 */
arch_move_cursor:
	push ebp
	mov ebp, esp
	
	push eax
	push ebx
	push edx

	xor eax, eax
	xor ebx, ebx
	xor edx, edx

	/*
	 * Function number is 0x2
	 */
	mov ah, 0x2

	/*
	 * DX register should contains
	 * coordinats
	 */
	mov ebx, [ebp + 0x8]
	and bx, 0xFF
	or dx, bx

	mov ebx, [ebp + 0xC]
	and bx, 0xFF
	shl bx, 0x8
	or dx, bx

	/*
	 * set cursor into position
	 */
	mov bh, 0x0
	int 0x10

	pop edx
	pop ebx
	pop eax

	mov esp, ebp
	pop ebp
	ret

arch_get_cursor_pos:
	push ebp
	mov ebp, esp

	push ebx
	push edx
	push ecx

	xor eax, eax
	xor ebx, ebx

	mov ah, 0x3
	int 0x10
	mov ax, dx
	
	pop ebx
	pop edx
	pop ecx
	
	mov esp, ebp
	pop ebp
	ret
