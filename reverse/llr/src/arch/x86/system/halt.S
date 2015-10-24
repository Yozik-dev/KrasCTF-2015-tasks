	.code16gcc
	.text
	.intel_syntax noprefix

	.global arch_halt

arch_halt:
	push ebp
	mov ebp, esp

	cli
	hlt
stop:
	jmp stop

	mov esp, ebp
	pop ebp
	ret
