#include "../asm.h"
/*  This file contains code to do profiling.  */
#ifndef __A4__
	.weak	__profile_frequency_value
	.global __profile_frequency_value
	.set	__profile_frequency_value, 1000
	.text
	.balign	4
	.global __profile_frequency
	FUNC(__profile_frequency)
__profile_frequency:
	mov_s	r0,__profile_frequency_value
	j_s	[blink]
	ENDFUNC(__profile_frequency)

#endif /* !A4 */
