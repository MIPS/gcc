#  This file contains the stack frame setup for contents of the .fini and
# .init sections.

	.section .init
	.global _init
	.word 0
_init:
	push_s	blink

	.section .fini
	.global _fini
	.word 0
_fini:
	push_s	blink
