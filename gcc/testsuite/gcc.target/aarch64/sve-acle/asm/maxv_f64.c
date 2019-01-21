/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxv_x0_f64:
**	fmaxv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (maxv_x0_f64, float64_t, svfloat64_t,
		  x0 = svmaxv_f64 (p0, z0),
		  x0 = svmaxv (p0, z0))

/*
** maxv_d0_f64_tied:
**	fmaxv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (maxv_d0_f64_tied, float64_t, svfloat64_t,
		  d0 = svmaxv_f64 (p0, z0),
		  d0 = svmaxv (p0, z0))

/*
** maxv_d0_f64_untied:
**	fmaxv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (maxv_d0_f64_untied, float64_t, svfloat64_t,
		  d0 = svmaxv_f64 (p0, z1),
		  d0 = svmaxv (p0, z1))
