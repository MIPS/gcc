/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpge_wide_s32_tied:
**	cmpge	p0\.s, p0/z, z0\.s, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpge_wide_s32_tied, svint32_t, svint64_t,
		     p0 = svcmpge_wide_s32 (p0, z0, z1),
		     p0 = svcmpge_wide (p0, z0, z1))

/*
** cmpge_wide_s32_untied:
**	cmpge	p0\.s, p1/z, z0\.s, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpge_wide_s32_untied, svint32_t, svint64_t,
		     p0 = svcmpge_wide_s32 (p1, z0, z1),
		     p0 = svcmpge_wide (p1, z0, z1))
