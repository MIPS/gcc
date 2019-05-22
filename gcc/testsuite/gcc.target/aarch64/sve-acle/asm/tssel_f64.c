/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tssel_f64_tied1:
**	ftssel	z0\.d, z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (tssel_f64_tied1, svfloat64_t, svuint64_t,
	     z0 = svtssel_f64 (z0, z16),
	     z0 = svtssel (z0, z16))

/*
** tssel_f64_tied2:
**	ftssel	z16\.d, z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (tssel_f64_tied2, svfloat64_t, svuint64_t,
	     z16_res = svtssel_f64 (z0, z16),
	     z16_res = svtssel (z0, z16))

/*
** tssel_f64_untied:
**	ftssel	z0\.d, z1\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (tssel_f64_untied, svfloat64_t, svuint64_t,
	     z0 = svtssel_f64 (z1, z16),
	     z0 = svtssel (z1, z16))
