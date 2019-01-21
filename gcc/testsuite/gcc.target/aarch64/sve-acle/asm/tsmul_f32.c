/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tsmul_f32_tied1:
**	ftsmul	z0\.s, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tsmul_f32_tied1, svfloat32_t, svuint32_t,
	     z0 = svtsmul_f32 (z0, z6),
	     z0 = svtsmul (z0, z6))

/*
** tsmul_f32_tied2:
**	ftsmul	z6\.s, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tsmul_f32_tied2, svfloat32_t, svuint32_t,
	     z6_res = svtsmul_f32 (z0, z6),
	     z6_res = svtsmul (z0, z6))

/*
** tsmul_f32_untied:
**	ftsmul	z0\.s, z1\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tsmul_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svtsmul_f32 (z1, z6),
	     z0 = svtsmul (z1, z6))
