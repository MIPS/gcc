#include "../asm.h"
#include "auxreg.h"
/*  This file contains code to do profiling.  */
#ifndef __A4__
	.weak	__profile_timer_cycles
	.global __profile_timer_cycles
	.set	__profile_timer_cycles, 200
	.text
	; For Arctangent-A5, if no data cache is present, a read of the
	; cache build register returns the ID register.  For ARC600 and
	; later, the version field will be zero.
	.global	__dcache_linesz
	.balign	4
__dcache_linesz:
	lr	r12,[D_CACHE_BUILD]
	extb_s	r0,r12
	breq_s	r0,0,.Lsz_nocache
	brge	r0,0x20,.Lsz_havecache
	lr	r0,[IDENTITY]
	breq	r12,r0,.Lsz_nocache
.Lsz_havecache:
	lsr_s	r12,r12,16
	mov_s	r0,16
	bmsk_s	r12,r12,3
	asl_s	r0,r0,r12
	j_s	[blink]
.Lsz_nocache:
	mov_s	r0,1
	j_s	[blink]
#endif /* !A4 */
