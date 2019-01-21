/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_s32_tied1:
**	sunpklo	z6\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpklo_s32_tied1, svint32_t, svint16_t,
	     z6_res = svunpklo_s32 (z6),
	     z6_res = svunpklo (z6))

/*
** unpklo_s32_untied:
**	sunpklo	z0\.s, z6\.h
**	ret
*/
TEST_DUAL_Z (unpklo_s32_untied, svint32_t, svint16_t,
	     z0 = svunpklo_s32 (z6),
	     z0 = svunpklo (z6))
