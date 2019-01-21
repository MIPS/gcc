/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpeq_wide_s16_tied:
**	cmpeq	p0\.h, p0/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpeq_wide_s16_tied, svint16_t, svint64_t,
		     p0 = svcmpeq_wide_s16 (p0, z0, z1),
		     p0 = svcmpeq_wide (p0, z0, z1))

/*
** cmpeq_wide_s16_untied:
**	cmpeq	p0\.h, p1/z, z0\.h, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmpeq_wide_s16_untied, svint16_t, svint64_t,
		     p0 = svcmpeq_wide_s16 (p1, z0, z1),
		     p0 = svcmpeq_wide (p1, z0, z1))
