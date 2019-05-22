/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tssel_f32_tied1:
**	ftssel	z0\.s, z0\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tssel_f32_tied1, svfloat32_t, svuint32_t,
	     z0 = svtssel_f32 (z0, z16),
	     z0 = svtssel (z0, z16))

/*
** tssel_f32_tied2:
**	ftssel	z16\.s, z0\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tssel_f32_tied2, svfloat32_t, svuint32_t,
	     z16_res = svtssel_f32 (z0, z16),
	     z16_res = svtssel (z0, z16))

/*
** tssel_f32_untied:
**	ftssel	z0\.s, z1\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tssel_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svtssel_f32 (z1, z16),
	     z0 = svtssel (z1, z16))
