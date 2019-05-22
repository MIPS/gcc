/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cvt_u16_f16_m_tied1:
**	fcvtzu	z0\.h, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_m_tied1, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_m (z0, p0, z16),
	     z0 = svcvt_u16_m (z0, p0, z16))

/*
** cvt_u16_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	fcvtzu	z16\.h, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_m_tied2, svuint16_t, svfloat16_t,
	     z16_res = svcvt_u16_f16_m (z0, p0, z16),
	     z16_res = svcvt_u16_m (z0, p0, z16))

/*
** cvt_u16_f16_m_untied:
**	movprfx	z0, z1
**	fcvtzu	z0\.h, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_m_untied, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_m (z1, p0, z16),
	     z0 = svcvt_u16_m (z1, p0, z16))

/*
** cvt_u16_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16\.h, p0/z, \1\.h
**	fcvtzu	z16\.h, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_z_tied1, svuint16_t, svfloat16_t,
	     z16_res = svcvt_u16_f16_z (p0, z16),
	     z16_res = svcvt_u16_z (p0, z16))

/*
** cvt_u16_f16_z_untied:
**	movprfx	z0\.h, p0/z, z16\.h
**	fcvtzu	z0\.h, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_z_untied, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_z (p0, z16),
	     z0 = svcvt_u16_z (p0, z16))

/*
** cvt_u16_f16_x_tied1:
**	fcvtzu	z16\.h, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_x_tied1, svuint16_t, svfloat16_t,
	     z16_res = svcvt_u16_f16_x (p0, z16),
	     z16_res = svcvt_u16_x (p0, z16))

/*
** cvt_u16_f16_x_untied:
**	fcvtzu	z0\.h, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_x_untied, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_x (p0, z16),
	     z0 = svcvt_u16_x (p0, z16))
