/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpgt_wide_s8_tied:
**	cmpgt	p0\.b, p0/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpgt_wide_s8_tied, svint8_t, svint64_t,
		     p0 = svcmpgt_wide_s8 (p0, z0, z1),
		     p0 = svcmpgt_wide (p0, z0, z1))

/*
** cmpgt_wide_s8_untied:
**	cmpgt	p0\.b, p1/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpgt_wide_s8_untied, svint8_t, svint64_t,
		     p0 = svcmpgt_wide_s8 (p1, z0, z1),
		     p0 = svcmpgt_wide (p1, z0, z1))
