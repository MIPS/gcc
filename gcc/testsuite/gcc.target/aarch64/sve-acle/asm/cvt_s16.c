/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cvt_s16_f16_m_tied1:
**	fcvtzs	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_m_tied1, svint16_t, svfloat16_t,
	     z0 = svcvt_s16_f16_m (z0, p0, z6),
	     z0 = svcvt_s16_m (z0, p0, z6))

/*
** cvt_s16_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fcvtzs	z6\.h, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_m_tied2, svint16_t, svfloat16_t,
	     z6_res = svcvt_s16_f16_m (z0, p0, z6),
	     z6_res = svcvt_s16_m (z0, p0, z6))

/*
** cvt_s16_f16_m_untied:
**	movprfx	z0, z1
**	fcvtzs	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_m_untied, svint16_t, svfloat16_t,
	     z0 = svcvt_s16_f16_m (z1, p0, z6),
	     z0 = svcvt_s16_m (z1, p0, z6))

/*
** cvt_s16_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.h, p0/z, \1\.h
**	fcvtzs	z6\.h, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_z_tied1, svint16_t, svfloat16_t,
	     z6_res = svcvt_s16_f16_z (p0, z6),
	     z6_res = svcvt_s16_z (p0, z6))

/*
** cvt_s16_f16_z_untied:
**	movprfx	z0\.h, p0/z, z6\.h
**	fcvtzs	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_z_untied, svint16_t, svfloat16_t,
	     z0 = svcvt_s16_f16_z (p0, z6),
	     z0 = svcvt_s16_z (p0, z6))

/*
** cvt_s16_f16_x_tied1:
**	fcvtzs	z6\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_x_tied1, svint16_t, svfloat16_t,
	     z6_res = svcvt_s16_f16_x (p0, z6),
	     z6_res = svcvt_s16_x (p0, z6))

/*
** cvt_s16_f16_x_untied:
**	fcvtzs	z0\.h, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_s16_f16_x_untied, svint16_t, svfloat16_t,
	     z0 = svcvt_s16_f16_x (p0, z6),
	     z0 = svcvt_s16_x (p0, z6))
