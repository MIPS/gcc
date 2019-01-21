/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpgt_wide_s16_tied:
**	cmpgt	p0\.h, p0/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpgt_wide_s16_tied, svint16_t, svint64_t,
		     p0 = svcmpgt_wide_s16 (p0, z0, z1),
		     p0 = svcmpgt_wide (p0, z0, z1))

/*
** cmpgt_wide_s16_untied:
**	cmpgt	p0\.h, p1/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpgt_wide_s16_untied, svint16_t, svint64_t,
		     p0 = svcmpgt_wide_s16 (p1, z0, z1),
		     p0 = svcmpgt_wide (p1, z0, z1))
