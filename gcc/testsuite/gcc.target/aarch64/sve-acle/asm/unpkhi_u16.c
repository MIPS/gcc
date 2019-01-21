/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_u16_tied1:
**	uunpkhi	z6\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpkhi_u16_tied1, svuint16_t, svuint8_t,
	     z6_res = svunpkhi_u16 (z6),
	     z6_res = svunpkhi (z6))

/*
** unpkhi_u16_untied:
**	uunpkhi	z0\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpkhi_u16_untied, svuint16_t, svuint8_t,
	     z0 = svunpkhi_u16 (z6),
	     z0 = svunpkhi (z6))
