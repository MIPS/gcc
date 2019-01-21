/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpne_wide_s16_tied:
**	cmpne	p0\.h, p0/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpne_wide_s16_tied, svint16_t, svint64_t,
		     p0 = svcmpne_wide_s16 (p0, z0, z1),
		     p0 = svcmpne_wide (p0, z0, z1))

/*
** cmpne_wide_s16_untied:
**	cmpne	p0\.h, p1/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpne_wide_s16_untied, svint16_t, svint64_t,
		     p0 = svcmpne_wide_s16 (p1, z0, z1),
		     p0 = svcmpne_wide (p1, z0, z1))
