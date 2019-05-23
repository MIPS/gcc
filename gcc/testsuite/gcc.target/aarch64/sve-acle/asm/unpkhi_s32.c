/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_s32_tied1:
**	sunpkhi	z16\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_s32_tied1, svint32_t, svint16_t,
	     z16_res = svunpkhi_s32 (z16),
	     z16_res = svunpkhi (z16))

/*
** unpkhi_s32_untied:
**	sunpkhi	z0\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_s32_untied, svint32_t, svint16_t,
	     z0 = svunpkhi_s32 (z16),
	     z0 = svunpkhi (z16))
