/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** len_x0_f32:
**	cntw	x0
**	ret
*/
TEST_REDUCTION_X (len_x0_f32, uint64_t, svfloat32_t,
		  x0 = svlen_f32 (z0),
		  x0 = svlen (z0))

/*
** len_d0_f32_tied:
**	cntw	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_f32_tied, uint64_t, svfloat32_t,
		  d0 = svlen_f32 (z0),
		  d0 = svlen (z0))

/*
** len_d0_f32_untied:
**	cntw	(x[0-9]+)
**	fmov	d0, \1
**	ret
*/
TEST_REDUCTION_D (len_d0_f32_untied, uint64_t, svfloat32_t,
		  d0 = svlen_f32 (z1),
		  d0 = svlen (z1))
