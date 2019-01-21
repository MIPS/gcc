/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tssel_f16_tied1:
**	ftssel	z0\.h, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (tssel_f16_tied1, svfloat16_t, svuint16_t,
	     z0 = svtssel_f16 (z0, z6),
	     z0 = svtssel (z0, z6))

/*
** tssel_f16_tied2:
**	ftssel	z6\.h, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (tssel_f16_tied2, svfloat16_t, svuint16_t,
	     z6_res = svtssel_f16 (z0, z6),
	     z6_res = svtssel (z0, z6))

/*
** tssel_f16_untied:
**	ftssel	z0\.h, z1\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (tssel_f16_untied, svfloat16_t, svuint16_t,
	     z0 = svtssel_f16 (z1, z6),
	     z0 = svtssel (z1, z6))
