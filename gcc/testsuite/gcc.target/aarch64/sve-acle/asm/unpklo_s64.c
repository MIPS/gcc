/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_s64_tied1:
**	sunpklo	z16\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpklo_s64_tied1, svint64_t, svint32_t,
	     z16_res = svunpklo_s64 (z16),
	     z16_res = svunpklo (z16))

/*
** unpklo_s64_untied:
**	sunpklo	z0\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpklo_s64_untied, svint64_t, svint32_t,
	     z0 = svunpklo_s64 (z16),
	     z0 = svunpklo (z16))
