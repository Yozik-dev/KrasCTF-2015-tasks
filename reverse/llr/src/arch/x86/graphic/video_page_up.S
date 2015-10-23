	.code16gcc
	.text
	.intel_syntax noprefix

	.global arch_move_active_video_page_up

arch_move_active_video_page_up:
	push ebp
	mov ebp, esp

	push ebx
	push edx
	push ecx
	push edx

	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx

	mov ecx, [ebp + 0x8]
	and ecx, 0xFFFF
	mov edx, [ebp + 0xC]
	and edx, 0xFFFF
	mov eax, [ebp + 0x10]
	and eax, 0xFF
	mov ah, 0x6
	int 0x10

	pop edx
	pop ecx
	pop edx
	pop ebx

	mov esp, ebp
	pop ebp
	ret