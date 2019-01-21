/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxv_x0_f32:
**	fmaxv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (maxv_x0_f32, float32_t, svfloat32_t,
		  x0 = svmaxv_f32 (p0, z0),
		  x0 = svmaxv (p0, z0))

/*
** maxv_d0_f32_tied:
**	fmaxv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (maxv_d0_f32_tied, float32_t, svfloat32_t,
		  d0 = svmaxv_f32 (p0, z0),
		  d0 = svmaxv (p0, z0))

/*
** maxv_d0_f32_untied:
**	fmaxv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (maxv_d0_f32_untied, float32_t, svfloat32_t,
		  d0 = svmaxv_f32 (p0, z1),
		  d0 = svmaxv (p0, z1))
