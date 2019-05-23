/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_u32_tied1:
**	uunpklo	z16\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpklo_u32_tied1, svuint32_t, svuint16_t,
	     z16_res = svunpklo_u32 (z16),
	     z16_res = svunpklo (z16))

/*
** unpklo_u32_untied:
**	uunpklo	z0\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpklo_u32_untied, svuint32_t, svuint16_t,
	     z0 = svunpklo_u32 (z16),
	     z0 = svunpklo (z16))
