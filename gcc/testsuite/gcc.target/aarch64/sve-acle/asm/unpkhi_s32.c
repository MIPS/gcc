/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_s32_tied1:
**	sunpkhi	z6\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_s32_tied1, svint32_t, svint16_t,
	     z6_res = svunpkhi_s32 (z6),
	     z6_res = svunpkhi (z6))

/*
** unpkhi_s32_untied:
**	sunpkhi	z0\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpkhi_s32_untied, svint32_t, svint16_t,
	     z0 = svunpkhi_s32 (z6),
	     z0 = svunpkhi (z6))
