/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxnmv_x0_f16:
**	fmaxnmv	h([0-9]+), p0, z0\.h
**	umov	w0, v\1\.h\[0\]
**	ret
*/
TEST_REDUCTION_X (maxnmv_x0_f16, float16_t, svfloat16_t,
		  x0 = svmaxnmv_f16 (p0, z0),
		  x0 = svmaxnmv (p0, z0))

/*
** maxnmv_d0_f16_tied:
**	fmaxnmv	h0, p0, z0\.h
**	ret
*/
TEST_REDUCTION_D (maxnmv_d0_f16_tied, float16_t, svfloat16_t,
		  d0 = svmaxnmv_f16 (p0, z0),
		  d0 = svmaxnmv (p0, z0))

/*
** maxnmv_d0_f16_untied:
**	fmaxnmv	h0, p0, z1\.h
**	ret
*/
TEST_REDUCTION_D (maxnmv_d0_f16_untied, float16_t, svfloat16_t,
		  d0 = svmaxnmv_f16 (p0, z1),
		  d0 = svmaxnmv (p0, z1))
