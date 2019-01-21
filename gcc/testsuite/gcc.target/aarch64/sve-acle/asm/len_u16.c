/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** len_x0_u16:
**	cnth	x0
**	ret
*/
TEST_REDUCTION_X (len_x0_u16, uint64_t, svuint16_t,
		  x0 = svlen_u16 (z0),
		  x0 = svlen (z0))

/*
** len_d0_u16_tied:
**	cnth	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_u16_tied, uint64_t, svuint16_t,
		  d0 = svlen_u16 (z0),
		  d0 = svlen (z0))

/*
** len_d0_u16_untied:
**	cnth	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_u16_untied, uint64_t, svuint16_t,
		  d0 = svlen_u16 (z1),
		  d0 = svlen (z1))
