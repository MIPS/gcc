	.section ".rodata","a"
	.globl wrapper
wrapper:
	.incbin "run/wrap.bin"
	.balign 16
	.size wrapper,.-wrapper

	.globl wrapper_len
wrapper_len:
	.long .-wrapper
	.size wrapper_len,.-wrapper_len

	.section .note.GNU-stack,"",@progbits
