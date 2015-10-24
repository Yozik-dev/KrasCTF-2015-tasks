	.text
	.code16gcc
	.intel_syntax noprefix

	.global arch_getc

arch_getc:
	push ebp
	mov ebp, esp

	xor eax, eax
	mov ah, 0
	int 0x16

	mov esp, ebp
	pop ebp
	ret
