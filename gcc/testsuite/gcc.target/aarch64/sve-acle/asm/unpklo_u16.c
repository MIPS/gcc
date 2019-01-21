/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_u16_tied1:
**	uunpklo	z6\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpklo_u16_tied1, svuint16_t, svuint8_t,
	     z6_res = svunpklo_u16 (z6),
	     z6_res = svunpklo (z6))

/*
** unpklo_u16_untied:
**	uunpklo	z0\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpklo_u16_untied, svuint16_t, svuint8_t,
	     z0 = svunpklo_u16 (z6),
	     z0 = svunpklo (z6))
