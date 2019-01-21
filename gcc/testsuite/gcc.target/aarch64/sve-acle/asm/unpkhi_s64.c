/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_s64_tied1:
**	sunpkhi	z6\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_s64_tied1, svint64_t, svint32_t,
	     z6_res = svunpkhi_s64 (z6),
	     z6_res = svunpkhi (z6))

/*
** unpkhi_s64_untied:
**	sunpkhi	z0\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_s64_untied, svint64_t, svint32_t,
	     z0 = svunpkhi_s64 (z6),
	     z0 = svunpkhi (z6))
