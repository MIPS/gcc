/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_s16_tied1:
**	sunpklo	z16\.h, z16\.b
**	ret
*/
TEST_DUAL_Z (unpklo_s16_tied1, svint16_t, svint8_t,
	     z16_res = svunpklo_s16 (z16),
	     z16_res = svunpklo (z16))

/*
** unpklo_s16_untied:
**	sunpklo	z0\.h, z16\.b
**	ret
*/
TEST_DUAL_Z (unpklo_s16_untied, svint16_t, svint8_t,
	     z0 = svunpklo_s16 (z16),
	     z0 = svunpklo (z16))
