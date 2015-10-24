	.text
	.code16gcc
	.intel_syntax noprefix
	
	.extern start

	.global kernel_entry
	
kernel_entry:
	call start