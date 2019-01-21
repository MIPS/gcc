/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** minv_x0_f64:
**	fminv	(d[0-9]+), p0, z0\.d
**	fmov	x0, \1
**	ret
*/
TEST_REDUCTION_X (minv_x0_f64, float64_t, svfloat64_t,
		  x0 = svminv_f64 (p0, z0),
		  x0 = svminv (p0, z0))

/*
** minv_d0_f64_tied:
**	fminv	d0, p0, z0\.d
**	ret
*/
TEST_REDUCTION_D (minv_d0_f64_tied, float64_t, svfloat64_t,
		  d0 = svminv_f64 (p0, z0),
		  d0 = svminv (p0, z0))

/*
** minv_d0_f64_untied:
**	fminv	d0, p0, z1\.d
**	ret
*/
TEST_REDUCTION_D (minv_d0_f64_untied, float64_t, svfloat64_t,
		  d0 = svminv_f64 (p0, z1),
		  d0 = svminv (p0, z1))
