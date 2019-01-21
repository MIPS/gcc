/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_u32_tied1:
**	uunpklo	z6\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpklo_u32_tied1, svuint32_t, svuint16_t,
	     z6_res = svunpklo_u32 (z6),
	     z6_res = svunpklo (z6))

/*
** unpklo_u32_untied:
**	uunpklo	z0\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpklo_u32_untied, svuint32_t, svuint16_t,
	     z0 = svunpklo_u32 (z6),
	     z0 = svunpklo (z6))
