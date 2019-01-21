/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** minnmv_x0_f32:
**	fminnmv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (minnmv_x0_f32, float32_t, svfloat32_t,
		  x0 = svminnmv_f32 (p0, z0),
		  x0 = svminnmv (p0, z0))

/*
** minnmv_d0_f32_tied:
**	fminnmv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (minnmv_d0_f32_tied, float32_t, svfloat32_t,
		  d0 = svminnmv_f32 (p0, z0),
		  d0 = svminnmv (p0, z0))

/*
** minnmv_d0_f32_untied:
**	fminnmv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (minnmv_d0_f32_untied, float32_t, svfloat32_t,
		  d0 = svminnmv_f32 (p0, z1),
		  d0 = svminnmv (p0, z1))
