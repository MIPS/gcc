/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** len_x0_s16:
**	cnth	x0
**	ret
*/
TEST_REDUCTION_X (len_x0_s16, uint64_t, svint16_t,
		  x0 = svlen_s16 (z0),
		  x0 = svlen (z0))

/*
** len_d0_s16_tied:
**	cnth	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_s16_tied, uint64_t, svint16_t,
		  d0 = svlen_s16 (z0),
		  d0 = svlen (z0))

/*
** len_d0_s16_untied:
**	cnth	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_s16_untied, uint64_t, svint16_t,
		  d0 = svlen_s16 (z1),
		  d0 = svlen (z1))
