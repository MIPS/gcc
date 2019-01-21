/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxnmv_x0_f32:
**	fmaxnmv	(s[0-9]+), p0, z0\.s
**	fmov	w0, \1
**	ret
*/
TEST_REDUCTION_X (maxnmv_x0_f32, float32_t, svfloat32_t,
		  x0 = svmaxnmv_f32 (p0, z0),
		  x0 = svmaxnmv (p0, z0))

/*
** maxnmv_d0_f32_tied:
**	fmaxnmv	s0, p0, z0\.s
**	ret
*/
TEST_REDUCTION_D (maxnmv_d0_f32_tied, float32_t, svfloat32_t,
		  d0 = svmaxnmv_f32 (p0, z0),
		  d0 = svmaxnmv (p0, z0))

/*
** maxnmv_d0_f32_untied:
**	fmaxnmv	s0, p0, z1\.s
**	ret
*/
TEST_REDUCTION_D (maxnmv_d0_f32_untied, float32_t, svfloat32_t,
		  d0 = svmaxnmv_f32 (p0, z1),
		  d0 = svmaxnmv (p0, z1))
