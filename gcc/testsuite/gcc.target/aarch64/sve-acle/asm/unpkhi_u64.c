/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_u64_tied1:
**	uunpkhi	z16\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_u64_tied1, svuint64_t, svuint32_t,
	     z16_res = svunpkhi_u64 (z16),
	     z16_res = svunpkhi (z16))

/*
** unpkhi_u64_untied:
**	uunpkhi	z0\.d, z16\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_u64_untied, svuint64_t, svuint32_t,
	     z0 = svunpkhi_u64 (z16),
	     z0 = svunpkhi (z16))
