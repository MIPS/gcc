/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_u64_tied1:
**	uunpkhi	z6\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_u64_tied1, svuint64_t, svuint32_t,
	     z6_res = svunpkhi_u64 (z6),
	     z6_res = svunpkhi (z6))

/*
** unpkhi_u64_untied:
**	uunpkhi	z0\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpkhi_u64_untied, svuint64_t, svuint32_t,
	     z0 = svunpkhi_u64 (z6),
	     z0 = svunpkhi (z6))
