/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmple_wide_s8_tied:
**	cmple	p0\.b, p0/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_s8_tied, svint8_t, svint64_t,
		     p0 = svcmple_wide_s8 (p0, z0, z1),
		     p0 = svcmple_wide (p0, z0, z1))

/*
** cmple_wide_s8_untied:
**	cmple	p0\.b, p1/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_s8_untied, svint8_t, svint64_t,
		     p0 = svcmple_wide_s8 (p1, z0, z1),
		     p0 = svcmple_wide (p1, z0, z1))
