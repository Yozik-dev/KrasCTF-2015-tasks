	.code16gcc
	.text
	.intel_syntax noprefix

	.global arch_init_display

arch_init_display:
	push ebp
	mov ebp, esp

	push ebx

	xor eax, eax
	xor ebx, ebx
	mov ebx, [ebp + 8]
	and ebx, 0xFFFF

	mov ax, 0x4F02
	int 0x10

	cmp ax, 0x4F
	jnz failed
	
	xor eax, eax
	jmp exit
	
failed:
	mov eax, 1

exit:
	pop ebx

	mov esp, ebp
	pop ebp
	ret
