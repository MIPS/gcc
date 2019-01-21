/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpne_wide_u16_tied:
**	cmpne	p0\.h, p0/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpne_wide_u16_tied, svuint16_t, svuint64_t,
		     p0 = svcmpne_wide_u16 (p0, z0, z1),
		     p0 = svcmpne_wide (p0, z0, z1))

/*
** cmpne_wide_u16_untied:
**	cmpne	p0\.h, p1/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpne_wide_u16_untied, svuint16_t, svuint64_t,
		     p0 = svcmpne_wide_u16 (p1, z0, z1),
		     p0 = svcmpne_wide (p1, z0, z1))
