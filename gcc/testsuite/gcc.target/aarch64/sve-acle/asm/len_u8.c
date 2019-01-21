/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** len_x0_u8:
**	cntb	x0
**	ret
*/
TEST_REDUCTION_X (len_x0_u8, uint64_t, svuint8_t,
		  x0 = svlen_u8 (z0),
		  x0 = svlen (z0))

/*
** len_d0_u8_tied:
**	cntb	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_u8_tied, uint64_t, svuint8_t,
		  d0 = svlen_u8 (z0),
		  d0 = svlen (z0))

/*
** len_d0_u8_untied:
**	cntb	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_u8_untied, uint64_t, svuint8_t,
		  d0 = svlen_u8 (z1),
		  d0 = svlen (z1))
