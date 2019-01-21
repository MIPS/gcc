/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpeq_wide_s8_tied:
**	cmpeq	p0\.b, p0/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpeq_wide_s8_tied, svint8_t, svint64_t,
		     p0 = svcmpeq_wide_s8 (p0, z0, z1),
		     p0 = svcmpeq_wide (p0, z0, z1))

/*
** cmpeq_wide_s8_untied:
**	cmpeq	p0\.b, p1/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpeq_wide_s8_untied, svint8_t, svint64_t,
		     p0 = svcmpeq_wide_s8 (p1, z0, z1),
		     p0 = svcmpeq_wide (p1, z0, z1))
