/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_s64_tied1:
**	sunpklo	z6\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpklo_s64_tied1, svint64_t, svint32_t,
	     z6_res = svunpklo_s64 (z6),
	     z6_res = svunpklo (z6))

/*
** unpklo_s64_untied:
**	sunpklo	z0\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpklo_s64_untied, svint64_t, svint32_t,
	     z0 = svunpklo_s64 (z6),
	     z0 = svunpklo (z6))
