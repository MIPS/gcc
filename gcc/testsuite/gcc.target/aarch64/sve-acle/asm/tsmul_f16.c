/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tsmul_f16_tied1:
**	ftsmul	z0\.h, z0\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (tsmul_f16_tied1, svfloat16_t, svuint16_t,
	     z0 = svtsmul_f16 (z0, z16),
	     z0 = svtsmul (z0, z16))

/*
** tsmul_f16_tied2:
**	ftsmul	z16\.h, z0\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (tsmul_f16_tied2, svfloat16_t, svuint16_t,
	     z16_res = svtsmul_f16 (z0, z16),
	     z16_res = svtsmul (z0, z16))

/*
** tsmul_f16_untied:
**	ftsmul	z0\.h, z1\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (tsmul_f16_untied, svfloat16_t, svuint16_t,
	     z0 = svtsmul_f16 (z1, z16),
	     z0 = svtsmul (z1, z16))
