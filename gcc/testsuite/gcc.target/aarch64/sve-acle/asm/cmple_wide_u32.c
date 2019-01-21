/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmple_wide_u32_tied:
**	cmpls	p0\.s, p0/z, z0\.s, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_u32_tied, svuint32_t, svuint64_t,
		     p0 = svcmple_wide_u32 (p0, z0, z1),
		     p0 = svcmple_wide (p0, z0, z1))

/*
** cmple_wide_u32_untied:
**	cmpls	p0\.s, p1/z, z0\.s, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_u32_untied, svuint32_t, svuint64_t,
		     p0 = svcmple_wide_u32 (p1, z0, z1),
		     p0 = svcmple_wide (p1, z0, z1))
