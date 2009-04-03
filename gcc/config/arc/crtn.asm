# This file just makes sure that the .fini and .init sections do in
# fact return. This file is the last thing linked into any executable.

	.section .init
	pop_s	blink
	j_s	[blink]


	.section .fini
	pop_s	blink
	j_s	[blink]
