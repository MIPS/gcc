/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** len_x0_f16:
**	cnth	x0
**	ret
*/
TEST_REDUCTION_X (len_x0_f16, uint64_t, svfloat16_t,
		  x0 = svlen_f16 (z0),
		  x0 = svlen (z0))

/*
** len_d0_f16_tied:
**	cnth	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_f16_tied, uint64_t, svfloat16_t,
		  d0 = svlen_f16 (z0),
		  d0 = svlen (z0))

/*
** len_d0_f16_untied:
**	cnth	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_f16_untied, uint64_t, svfloat16_t,
		  d0 = svlen_f16 (z1),
		  d0 = svlen (z1))
