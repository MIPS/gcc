/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmplt_wide_u8_tied:
**	cmplo	p0\.b, p0/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmplt_wide_u8_tied, svuint8_t, svuint64_t,
		     p0 = svcmplt_wide_u8 (p0, z0, z1),
		     p0 = svcmplt_wide (p0, z0, z1))

/*
** cmplt_wide_u8_untied:
**	cmplo	p0\.b, p1/z, z0\.b, z1\.d
**	ret
*/
TEST_COMPARE_DUAL_Z (cmplt_wide_u8_untied, svuint8_t, svuint64_t,
		     p0 = svcmplt_wide_u8 (p1, z0, z1),
		     p0 = svcmplt_wide (p1, z0, z1))
