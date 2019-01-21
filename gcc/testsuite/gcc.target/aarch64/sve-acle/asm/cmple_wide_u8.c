/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmple_wide_u8_tied:
**	cmpls	p0\.b, p0/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_u8_tied, svuint8_t, svuint64_t,
		     p0 = svcmple_wide_u8 (p0, z0, z1),
		     p0 = svcmple_wide (p0, z0, z1))

/*
** cmple_wide_u8_untied:
**	cmpls	p0\.b, p1/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmple_wide_u8_untied, svuint8_t, svuint64_t,
		     p0 = svcmple_wide_u8 (p1, z0, z1),
		     p0 = svcmple_wide (p1, z0, z1))
