/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_u64_tied1:
**	uunpklo	z16\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpklo_u64_tied1, svuint64_t, svuint32_t,
	     z16_res = svunpklo_u64 (z16),
	     z16_res = svunpklo (z16))

/*
** unpklo_u64_untied:
**	uunpklo	z0\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpklo_u64_untied, svuint64_t, svuint32_t,
	     z0 = svunpklo_u64 (z16),
	     z0 = svunpklo (z16))
