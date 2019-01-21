/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpge_wide_s8_tied:
**	cmpge	p0\.b, p0/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpge_wide_s8_tied, svint8_t, svint64_t,
		     p0 = svcmpge_wide_s8 (p0, z0, z1),
		     p0 = svcmpge_wide (p0, z0, z1))

/*
** cmpge_wide_s8_untied:
**	cmpge	p0\.b, p1/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpge_wide_s8_untied, svint8_t, svint64_t,
		     p0 = svcmpge_wide_s8 (p1, z0, z1),
		     p0 = svcmpge_wide (p1, z0, z1))
