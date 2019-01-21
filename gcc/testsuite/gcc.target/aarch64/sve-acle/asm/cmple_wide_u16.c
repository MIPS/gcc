/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmple_wide_u16_tied:
**	cmpls	p0\.h, p0/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_u16_tied, svuint16_t, svuint64_t,
		     p0 = svcmple_wide_u16 (p0, z0, z1),
		     p0 = svcmple_wide (p0, z0, z1))

/*
** cmple_wide_u16_untied:
**	cmpls	p0\.h, p1/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_u16_untied, svuint16_t, svuint64_t,
		     p0 = svcmple_wide_u16 (p1, z0, z1),
		     p0 = svcmple_wide (p1, z0, z1))
