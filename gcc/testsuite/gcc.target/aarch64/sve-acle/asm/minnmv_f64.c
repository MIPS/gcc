/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** minnmv_x0_f64:
**	fminnmv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (minnmv_x0_f64, float64_t, svfloat64_t,
		  x0 = svminnmv_f64 (p0, z0),
		  x0 = svminnmv (p0, z0))

/*
** minnmv_d0_f64_tied:
**	fminnmv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (minnmv_d0_f64_tied, float64_t, svfloat64_t,
		  d0 = svminnmv_f64 (p0, z0),
		  d0 = svminnmv (p0, z0))

/*
** minnmv_d0_f64_untied:
**	fminnmv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (minnmv_d0_f64_untied, float64_t, svfloat64_t,
		  d0 = svminnmv_f64 (p0, z1),
		  d0 = svminnmv (p0, z1))
