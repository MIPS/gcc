/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_u32_tied1:
**	uunpkhi	z6\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_u32_tied1, svuint32_t, svuint16_t,
	     z6_res = svunpkhi_u32 (z6),
	     z6_res = svunpkhi (z6))

/*
** unpkhi_u32_untied:
**	uunpkhi	z0\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_u32_untied, svuint32_t, svuint16_t,
	     z0 = svunpkhi_u32 (z6),
	     z0 = svunpkhi (z6))
