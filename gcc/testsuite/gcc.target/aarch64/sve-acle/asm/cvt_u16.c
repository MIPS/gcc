/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cvt_u16_f16_m_tied1:
**	fcvtzu	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_m_tied1, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_m (z0, p0, z6),
	     z0 = svcvt_u16_m (z0, p0, z6))

/*
** cvt_u16_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fcvtzu	z6\.h, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_m_tied2, svuint16_t, svfloat16_t,
	     z6_res = svcvt_u16_f16_m (z0, p0, z6),
	     z6_res = svcvt_u16_m (z0, p0, z6))

/*
** cvt_u16_f16_m_untied:
**	movprfx	z0, z1
**	fcvtzu	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_m_untied, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_m (z1, p0, z6),
	     z0 = svcvt_u16_m (z1, p0, z6))

/*
** cvt_u16_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.h, p0/z, \1\.h
**	fcvtzu	z6\.h, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_z_tied1, svuint16_t, svfloat16_t,
	     z6_res = svcvt_u16_f16_z (p0, z6),
	     z6_res = svcvt_u16_z (p0, z6))

/*
** cvt_u16_f16_z_untied:
**	movprfx	z0\.h, p0/z, z6\.h
**	fcvtzu	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_z_untied, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_z (p0, z6),
	     z0 = svcvt_u16_z (p0, z6))

/*
** cvt_u16_f16_x_tied1:
**	fcvtzu	z6\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_x_tied1, svuint16_t, svfloat16_t,
	     z6_res = svcvt_u16_f16_x (p0, z6),
	     z6_res = svcvt_u16_x (p0, z6))

/*
** cvt_u16_f16_x_untied:
**	fcvtzu	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_u16_f16_x_untied, svuint16_t, svfloat16_t,
	     z0 = svcvt_u16_f16_x (p0, z6),
	     z0 = svcvt_u16_x (p0, z6))
