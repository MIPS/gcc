/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lastb_x0_f16:
**	lastb	w0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_X (lastb_x0_f16, float16_t, svfloat16_t,
		  x0 = svlastb_f16 (p0, z0),
		  x0 = svlastb (p0, z0))

/*
** lastb_d0_f16_tied:
**	lastb	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (lastb_d0_f16_tied, float16_t, svfloat16_t,
		  d0 = svlastb_f16 (p0, z0),
		  d0 = svlastb (p0, z0))

/*
** lastb_d0_f16_untied:
**	lastb	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (lastb_d0_f16_untied, float16_t, svfloat16_t,
		  d0 = svlastb_f16 (p0, z1),
		  d0 = svlastb (p0, z1))
