	.text
	.code16gcc
	.intel_syntax noprefix

	.global arch_putc
	
arch_putc:
	push ebp
	mov ebp, esp

	push ebx
	
	xor ebx, ebx
	xor eax, eax
	
	mov eax, [ebp + 0x8]
	and eax, 0xFF

	mov ebx, [ebp + 0xC]
	and ebx, 0xFF

	test al, al
	jz null_char

	mov ah, 0xE
	int 0x10

null_char:

	pop ebx
	
	mov esp, ebp
	pop ebp
	ret
