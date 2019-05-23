/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_u16_tied1:
**	uunpklo	z16\.h, z16\.b
**	ret
*/
TEST_DUAL_Z (unpklo_u16_tied1, svuint16_t, svuint8_t,
	     z16_res = svunpklo_u16 (z16),
	     z16_res = svunpklo (z16))

/*
** unpklo_u16_untied:
**	uunpklo	z0\.h, z16\.b
**	ret
*/
TEST_DUAL_Z (unpklo_u16_untied, svuint16_t, svuint8_t,
	     z0 = svunpklo_u16 (z16),
	     z0 = svunpklo (z16))
