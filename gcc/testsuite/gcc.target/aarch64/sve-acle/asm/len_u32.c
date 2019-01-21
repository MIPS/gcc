/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** len_x0_u32:
**	cntw	x0
**	ret
*/
TEST_REDUCTION_X (len_x0_u32, uint64_t, svuint32_t,
		  x0 = svlen_u32 (z0),
		  x0 = svlen (z0))

/*
** len_d0_u32_tied:
**	cntw	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_u32_tied, uint64_t, svuint32_t,
		  d0 = svlen_u32 (z0),
		  d0 = svlen (z0))

/*
** len_d0_u32_untied:
**	cntw	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_u32_untied, uint64_t, svuint32_t,
		  d0 = svlen_u32 (z1),
		  d0 = svlen (z1))
