/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cvt_u64_f16_m_tied1:
**	fcvtzu	z0\.d, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u64_f16_m_tied1, svuint64_t, svfloat16_t,
	     z0 = svcvt_u64_f16_m (z0, p0, z16),
	     z0 = svcvt_u64_m (z0, p0, z16))

/*
** cvt_u64_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	fcvtzu	z16\.d, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_u64_f16_m_tied2, svuint64_t, svfloat16_t,
	     z16_res = svcvt_u64_f16_m (z0, p0, z16),
	     z16_res = svcvt_u64_m (z0, p0, z16))

/*
** cvt_u64_f16_m_untied:
**	movprfx	z0, z1
**	fcvtzu	z0\.d, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u64_f16_m_untied, svuint64_t, svfloat16_t,
	     z0 = svcvt_u64_f16_m (z1, p0, z16),
	     z0 = svcvt_u64_m (z1, p0, z16))

/*
** cvt_u64_f32_m_tied1:
**	fcvtzu	z0\.d, p0/m, z16\.s
**	ret
*/
TEST_DUAL_Z (cvt_u64_f32_m_tied1, svuint64_t, svfloat32_t,
	     z0 = svcvt_u64_f32_m (z0, p0, z16),
	     z0 = svcvt_u64_m (z0, p0, z16))

/*
** cvt_u64_f32_m_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	fcvtzu	z16\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_u64_f32_m_tied2, svuint64_t, svfloat32_t,
	     z16_res = svcvt_u64_f32_m (z0, p0, z16),
	     z16_res = svcvt_u64_m (z0, p0, z16))

/*
** cvt_u64_f32_m_untied:
**	movprfx	z0, z1
**	fcvtzu	z0\.d, p0/m, z16\.s
**	ret
*/
TEST_DUAL_Z (cvt_u64_f32_m_untied, svuint64_t, svfloat32_t,
	     z0 = svcvt_u64_f32_m (z1, p0, z16),
	     z0 = svcvt_u64_m (z1, p0, z16))

/*
** cvt_u64_f64_m_tied1:
**	fcvtzu	z0\.d, p0/m, z16\.d
**	ret
*/
TEST_DUAL_Z (cvt_u64_f64_m_tied1, svuint64_t, svfloat64_t,
	     z0 = svcvt_u64_f64_m (z0, p0, z16),
	     z0 = svcvt_u64_m (z0, p0, z16))

/*
** cvt_u64_f64_m_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16, z0
**	fcvtzu	z16\.d, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_u64_f64_m_tied2, svuint64_t, svfloat64_t,
	     z16_res = svcvt_u64_f64_m (z0, p0, z16),
	     z16_res = svcvt_u64_m (z0, p0, z16))

/*
** cvt_u64_f64_m_untied:
**	movprfx	z0, z1
**	fcvtzu	z0\.d, p0/m, z16\.d
**	ret
*/
TEST_DUAL_Z (cvt_u64_f64_m_untied, svuint64_t, svfloat64_t,
	     z0 = svcvt_u64_f64_m (z1, p0, z16),
	     z0 = svcvt_u64_m (z1, p0, z16))

/*
** cvt_u64_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16\.d, p0/z, \1\.d
**	fcvtzu	z16\.d, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_u64_f16_z_tied1, svuint64_t, svfloat16_t,
	     z16_res = svcvt_u64_f16_z (p0, z16),
	     z16_res = svcvt_u64_z (p0, z16))

/*
** cvt_u64_f16_z_untied:
**	movprfx	z0\.d, p0/z, z16\.d
**	fcvtzu	z0\.d, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u64_f16_z_untied, svuint64_t, svfloat16_t,
	     z0 = svcvt_u64_f16_z (p0, z16),
	     z0 = svcvt_u64_z (p0, z16))

/*
** cvt_u64_f32_z_tied1:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16\.d, p0/z, \1\.d
**	fcvtzu	z16\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_u64_f32_z_tied1, svuint64_t, svfloat32_t,
	     z16_res = svcvt_u64_f32_z (p0, z16),
	     z16_res = svcvt_u64_z (p0, z16))

/*
** cvt_u64_f32_z_untied:
**	movprfx	z0\.d, p0/z, z16\.d
**	fcvtzu	z0\.d, p0/m, z16\.s
**	ret
*/
TEST_DUAL_Z (cvt_u64_f32_z_untied, svuint64_t, svfloat32_t,
	     z0 = svcvt_u64_f32_z (p0, z16),
	     z0 = svcvt_u64_z (p0, z16))

/*
** cvt_u64_f64_z_tied1:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16\.d, p0/z, \1
**	fcvtzu	z16\.d, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_u64_f64_z_tied1, svuint64_t, svfloat64_t,
	     z16_res = svcvt_u64_f64_z (p0, z16),
	     z16_res = svcvt_u64_z (p0, z16))

/*
** cvt_u64_f64_z_untied:
**	movprfx	z0\.d, p0/z, z16\.d
**	fcvtzu	z0\.d, p0/m, z16\.d
**	ret
*/
TEST_DUAL_Z (cvt_u64_f64_z_untied, svuint64_t, svfloat64_t,
	     z0 = svcvt_u64_f64_z (p0, z16),
	     z0 = svcvt_u64_z (p0, z16))

/*
** cvt_u64_f16_x_tied1:
**	fcvtzu	z16\.d, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u64_f16_x_tied1, svuint64_t, svfloat16_t,
	     z16_res = svcvt_u64_f16_x (p0, z16),
	     z16_res = svcvt_u64_x (p0, z16))

/*
** cvt_u64_f16_x_untied:
**	fcvtzu	z0\.d, p0/m, z16\.h
**	ret
*/
TEST_DUAL_Z (cvt_u64_f16_x_untied, svuint64_t, svfloat16_t,
	     z0 = svcvt_u64_f16_x (p0, z16),
	     z0 = svcvt_u64_x (p0, z16))

/*
** cvt_u64_f32_x_tied1:
**	fcvtzu	z16\.d, p0/m, z16\.s
**	ret
*/
TEST_DUAL_Z (cvt_u64_f32_x_tied1, svuint64_t, svfloat32_t,
	     z16_res = svcvt_u64_f32_x (p0, z16),
	     z16_res = svcvt_u64_x (p0, z16))

/*
** cvt_u64_f32_x_untied:
**	fcvtzu	z0\.d, p0/m, z16\.s
**	ret
*/
TEST_DUAL_Z (cvt_u64_f32_x_untied, svuint64_t, svfloat32_t,
	     z0 = svcvt_u64_f32_x (p0, z16),
	     z0 = svcvt_u64_x (p0, z16))

/*
** cvt_u64_f64_x_tied1:
**	fcvtzu	z16\.d, p0/m, z16\.d
**	ret
*/
TEST_DUAL_Z (cvt_u64_f64_x_tied1, svuint64_t, svfloat64_t,
	     z16_res = svcvt_u64_f64_x (p0, z16),
	     z16_res = svcvt_u64_x (p0, z16))

/*
** cvt_u64_f64_x_untied:
**	fcvtzu	z0\.d, p0/m, z16\.d
**	ret
*/
TEST_DUAL_Z (cvt_u64_f64_x_untied, svuint64_t, svfloat64_t,
	     z0 = svcvt_u64_f64_x (p0, z16),
	     z0 = svcvt_u64_x (p0, z16))
