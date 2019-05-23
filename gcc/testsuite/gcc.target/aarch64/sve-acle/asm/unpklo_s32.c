/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_s32_tied1:
**	sunpklo	z16\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpklo_s32_tied1, svint32_t, svint16_t,
	     z16_res = svunpklo_s32 (z16),
	     z16_res = svunpklo (z16))

/*
** unpklo_s32_untied:
**	sunpklo	z0\.s, z16\.h
**	ret
*/
TEST_DUAL_Z (unpklo_s32_untied, svint32_t, svint16_t,
	     z0 = svunpklo_s32 (z16),
	     z0 = svunpklo (z16))
