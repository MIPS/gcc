/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_s64_tied1:
**	sunpkhi	z16\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_s64_tied1, svint64_t, svint32_t,
	     z16_res = svunpkhi_s64 (z16),
	     z16_res = svunpkhi (z16))

/*
** unpkhi_s64_untied:
**	sunpkhi	z0\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_s64_untied, svint64_t, svint32_t,
	     z0 = svunpkhi_s64 (z16),
	     z0 = svunpkhi (z16))
