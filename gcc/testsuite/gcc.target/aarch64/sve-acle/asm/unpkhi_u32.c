/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_u32_tied1:
**	uunpkhi	z16\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_u32_tied1, svuint32_t, svuint16_t,
	     z16_res = svunpkhi_u32 (z16),
	     z16_res = svunpkhi (z16))

/*
** unpkhi_u32_untied:
**	uunpkhi	z0\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_u32_untied, svuint32_t, svuint16_t,
	     z0 = svunpkhi_u32 (z16),
	     z0 = svunpkhi (z16))
