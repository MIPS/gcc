/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** scale_f16_m_tied1:
**	fscale	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_m_tied1, svfloat16_t, svint16_t,
	     z0 = svscale_f16_m (p0, z0, z6),
	     z0 = svscale_m (p0, z0, z6))

/*
** scale_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fscale	z6\.h, p0/m, z6\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_m_tied2, svfloat16_t, svint16_t,
	     z6_res = svscale_f16_m (p0, z0, z6),
	     z6_res = svscale_m (p0, z0, z6))

/*
** scale_f16_m_untied:
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_m_untied, svfloat16_t, svint16_t,
	     z0 = svscale_f16_m (p0, z1, z6),
	     z0 = svscale_m (p0, z1, z6))

/*
** scale_w0_f16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_w0_f16_m_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_m (p0, z0, x0),
		 z0 = svscale_m (p0, z0, x0))

/*
** scale_w0_f16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_w0_f16_m_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_m (p0, z1, x0),
		 z0 = svscale_m (p0, z1, x0))

/*
** scale_h4_f16_m_tied1:
**	mov	(z[0-9]+\.h), h4
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_h4_f16_m_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_m (p0, z0, d4),
		 z0 = svscale_m (p0, z0, d4))

/*
** scale_h4_f16_m_untied:
**	mov	(z[0-9]+\.h), h4
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_h4_f16_m_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_m (p0, z1, d4),
		 z0 = svscale_m (p0, z1, d4))

/*
** scale_3_f16_m_tied1:
**	mov	(z[0-9]+\.h), #3
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f16_m_tied1, svfloat16_t,
		z0 = svscale_n_f16_m (p0, z0, 3),
		z0 = svscale_m (p0, z0, 3))

/*
** scale_3_f16_m_untied:
**	mov	(z[0-9]+\.h), #3
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f16_m_untied, svfloat16_t,
		z0 = svscale_n_f16_m (p0, z1, 3),
		z0 = svscale_m (p0, z1, 3))

/*
** scale_m3_f16_m:
**	mov	(z[0-9]+\.h), #-3
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_m3_f16_m, svfloat16_t,
		z0 = svscale_n_f16_m (p0, z0, -3),
		z0 = svscale_m (p0, z0, -3))

/*
** scale_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fscale	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_z_tied1, svfloat16_t, svint16_t,
	     z0 = svscale_f16_z (p0, z0, z6),
	     z0 = svscale_z (p0, z0, z6))

/*
** scale_f16_z_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.h, p0/z, z0\.h
**	fscale	z6\.h, p0/m, z6\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_z_tied2, svfloat16_t, svint16_t,
	     z6_res = svscale_f16_z (p0, z0, z6),
	     z6_res = svscale_z (p0, z0, z6))

/*
** scale_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fscale	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_z_untied, svfloat16_t, svint16_t,
	     z0 = svscale_f16_z (p0, z1, z6),
	     z0 = svscale_z (p0, z1, z6))

/*
** scale_w0_f16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_w0_f16_z_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_z (p0, z0, x0),
		 z0 = svscale_z (p0, z0, x0))

/*
** scale_w0_f16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z1\.h
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_w0_f16_z_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_z (p0, z1, x0),
		 z0 = svscale_z (p0, z1, x0))

/*
** scale_h4_f16_z_tied1:
**	mov	(z[0-9]+\.h), h4
**	movprfx	z0\.h, p0/z, z0\.h
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_h4_f16_z_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_z (p0, z0, d4),
		 z0 = svscale_z (p0, z0, d4))

/*
** scale_h4_f16_z_untied:
**	mov	(z[0-9]+\.h), h4
**	movprfx	z0\.h, p0/z, z1\.h
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_h4_f16_z_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_z (p0, z1, d4),
		 z0 = svscale_z (p0, z1, d4))

/*
** scale_3_f16_z_tied1:
**	mov	(z[0-9]+\.h), #3
**	movprfx	z0\.h, p0/z, z0\.h
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f16_z_tied1, svfloat16_t,
		z0 = svscale_n_f16_z (p0, z0, 3),
		z0 = svscale_z (p0, z0, 3))

/*
** scale_3_f16_z_untied:
**	mov	(z[0-9]+\.h), #3
**	movprfx	z0\.h, p0/z, z1\.h
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f16_z_untied, svfloat16_t,
		z0 = svscale_n_f16_z (p0, z1, 3),
		z0 = svscale_z (p0, z1, 3))

/*
** scale_m3_f16_z:
**	mov	(z[0-9]+\.h), #-3
**	movprfx	z0\.h, p0/z, z0\.h
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_m3_f16_z, svfloat16_t,
		z0 = svscale_n_f16_z (p0, z0, -3),
		z0 = svscale_z (p0, z0, -3))

/*
** scale_f16_x_tied1:
**	fscale	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_x_tied1, svfloat16_t, svint16_t,
	     z0 = svscale_f16_x (p0, z0, z6),
	     z0 = svscale_x (p0, z0, z6))

/*
** scale_f16_x_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fscale	z6\.h, p0/m, z6\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_x_tied2, svfloat16_t, svint16_t,
	     z6_res = svscale_f16_x (p0, z0, z6),
	     z6_res = svscale_x (p0, z0, z6))

/*
** scale_f16_x_untied:
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (scale_f16_x_untied, svfloat16_t, svint16_t,
	     z0 = svscale_f16_x (p0, z1, z6),
	     z0 = svscale_x (p0, z1, z6))

/*
** scale_w0_f16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_w0_f16_x_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (p0, z0, x0),
		 z0 = svscale_x (p0, z0, x0))

/*
** scale_w0_f16_x_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_w0_f16_x_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (p0, z1, x0),
		 z0 = svscale_x (p0, z1, x0))

/*
** scale_h4_f16_x_tied1:
**	mov	(z[0-9]+\.h), h4
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_h4_f16_x_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (p0, z0, d4),
		 z0 = svscale_x (p0, z0, d4))

/*
** scale_h4_f16_x_untied:
**	mov	(z[0-9]+\.h), h4
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_h4_f16_x_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (p0, z1, d4),
		 z0 = svscale_x (p0, z1, d4))

/*
** scale_3_f16_x_tied1:
**	mov	(z[0-9]+\.h), #3
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f16_x_tied1, svfloat16_t,
		z0 = svscale_n_f16_x (p0, z0, 3),
		z0 = svscale_x (p0, z0, 3))

/*
** scale_3_f16_x_untied:
**	mov	(z[0-9]+\.h), #3
**	movprfx	z0, z1
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f16_x_untied, svfloat16_t,
		z0 = svscale_n_f16_x (p0, z1, 3),
		z0 = svscale_x (p0, z1, 3))

/*
** scale_m3_f16_x:
**	mov	(z[0-9]+\.h), #-3
**	fscale	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (scale_m3_f16_x, svfloat16_t,
		z0 = svscale_n_f16_x (p0, z0, -3),
		z0 = svscale_x (p0, z0, -3))

/*
** ptrue_scale_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fscale	z0\.h, \1/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (ptrue_scale_f16_x_tied1, svfloat16_t, svint16_t,
	     z0 = svscale_f16_x (svptrue_b16 (), z0, z6),
	     z0 = svscale_x (svptrue_b16 (), z0, z6))

/*
** ptrue_scale_f16_x_tied2:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fscale	z6\.h, \1/m, z6\.h, \2\.h
**	ret
*/
TEST_DUAL_Z (ptrue_scale_f16_x_tied2, svfloat16_t, svint16_t,
	     z6_res = svscale_f16_x (svptrue_b16 (), z0, z6),
	     z6_res = svscale_x (svptrue_b16 (), z0, z6))

/*
** ptrue_scale_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z0, z1
**	fscale	z0\.h, \1/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (ptrue_scale_f16_x_untied, svfloat16_t, svint16_t,
	     z0 = svscale_f16_x (svptrue_b16 (), z1, z6),
	     z0 = svscale_x (svptrue_b16 (), z1, z6))

/*
** ptrue_scale_w0_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), w0
**	fscale	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_w0_f16_x_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (svptrue_b16 (), z0, x0),
		 z0 = svscale_x (svptrue_b16 (), z0, x0))

/*
** ptrue_scale_w0_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	fscale	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_w0_f16_x_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (svptrue_b16 (), z1, x0),
		 z0 = svscale_x (svptrue_b16 (), z1, x0))

/*
** ptrue_scale_h4_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), h4
**	fscale	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_h4_f16_x_tied1, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (svptrue_b16 (), z0, d4),
		 z0 = svscale_x (svptrue_b16 (), z0, d4))

/*
** ptrue_scale_h4_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), h4
**	movprfx	z0, z1
**	fscale	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_h4_f16_x_untied, svfloat16_t, int16_t,
		 z0 = svscale_n_f16_x (svptrue_b16 (), z1, d4),
		 z0 = svscale_x (svptrue_b16 (), z1, d4))

/*
** ptrue_scale_3_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), #3
**	fscale	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_scale_3_f16_x_tied1, svfloat16_t,
		z0 = svscale_n_f16_x (svptrue_b16 (), z0, 3),
		z0 = svscale_x (svptrue_b16 (), z0, 3))

/*
** ptrue_scale_3_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), #3
**	movprfx	z0, z1
**	fscale	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_scale_3_f16_x_untied, svfloat16_t,
		z0 = svscale_n_f16_x (svptrue_b16 (), z1, 3),
		z0 = svscale_x (svptrue_b16 (), z1, 3))

/*
** ptrue_scale_m3_f16_x:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), #-3
**	fscale	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_scale_m3_f16_x, svfloat16_t,
		z0 = svscale_n_f16_x (svptrue_b16 (), z0, -3),
		z0 = svscale_x (svptrue_b16 (), z0, -3))
