/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tssel_f32_tied1:
**	ftssel	z0\.s, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tssel_f32_tied1, svfloat32_t, svuint32_t,
	     z0 = svtssel_f32 (z0, z6),
	     z0 = svtssel (z0, z6))

/*
** tssel_f32_tied2:
**	ftssel	z6\.s, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tssel_f32_tied2, svfloat32_t, svuint32_t,
	     z6_res = svtssel_f32 (z0, z6),
	     z6_res = svtssel (z0, z6))

/*
** tssel_f32_untied:
**	ftssel	z0\.s, z1\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tssel_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svtssel_f32 (z1, z6),
	     z0 = svtssel (z1, z6))
