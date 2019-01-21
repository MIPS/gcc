/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** maxnmv_x0_f64:
**	fmaxnmv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (maxnmv_x0_f64, float64_t, svfloat64_t,
		  x0 = svmaxnmv_f64 (p0, z0),
		  x0 = svmaxnmv (p0, z0))

/*
** maxnmv_d0_f64_tied:
**	fmaxnmv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (maxnmv_d0_f64_tied, float64_t, svfloat64_t,
		  d0 = svmaxnmv_f64 (p0, z0),
		  d0 = svmaxnmv (p0, z0))

/*
** maxnmv_d0_f64_untied:
**	fmaxnmv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (maxnmv_d0_f64_untied, float64_t, svfloat64_t,
		  d0 = svmaxnmv_f64 (p0, z1),
		  d0 = svmaxnmv (p0, z1))
