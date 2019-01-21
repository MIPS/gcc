/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** scale_f64_m_tied1:
**	fscale	z0\.d, p0/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (scale_f64_m_tied1, svfloat64_t, svint64_t,
	     z0 = svscale_f64_m (p0, z0, z6),
	     z0 = svscale_m (p0, z0, z6))

/*
** scale_f64_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	fscale	z6\.d, p0/m, z6\.d, \1
**	ret
*/
TEST_DUAL_Z (scale_f64_m_tied2, svfloat64_t, svint64_t,
	     z6_res = svscale_f64_m (p0, z0, z6),
	     z6_res = svscale_m (p0, z0, z6))

/*
** scale_f64_m_untied:
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (scale_f64_m_untied, svfloat64_t, svint64_t,
	     z0 = svscale_f64_m (p0, z1, z6),
	     z0 = svscale_m (p0, z1, z6))

/*
** scale_x0_f64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_x0_f64_m_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_m (p0, z0, x0),
		 z0 = svscale_m (p0, z0, x0))

/*
** scale_x0_f64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_x0_f64_m_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_m (p0, z1, x0),
		 z0 = svscale_m (p0, z1, x0))

/*
** scale_d4_f64_m_tied1:
**	mov	(z[0-9]+\.d), d4
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_d4_f64_m_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_m (p0, z0, d4),
		 z0 = svscale_m (p0, z0, d4))

/*
** scale_d4_f64_m_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_d4_f64_m_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_m (p0, z1, d4),
		 z0 = svscale_m (p0, z1, d4))

/*
** scale_3_f64_m_tied1:
**	mov	(z[0-9]+\.d), #3
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f64_m_tied1, svfloat64_t,
		z0 = svscale_n_f64_m (p0, z0, 3),
		z0 = svscale_m (p0, z0, 3))

/*
** scale_3_f64_m_untied:
**	mov	(z[0-9]+\.d), #3
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f64_m_untied, svfloat64_t,
		z0 = svscale_n_f64_m (p0, z1, 3),
		z0 = svscale_m (p0, z1, 3))

/*
** scale_m3_f64_m:
**	mov	(z[0-9]+\.d), #-3
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_m3_f64_m, svfloat64_t,
		z0 = svscale_n_f64_m (p0, z0, -3),
		z0 = svscale_m (p0, z0, -3))

/*
** scale_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fscale	z0\.d, p0/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (scale_f64_z_tied1, svfloat64_t, svint64_t,
	     z0 = svscale_f64_z (p0, z0, z6),
	     z0 = svscale_z (p0, z0, z6))

/*
** scale_f64_z_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.d, p0/z, z0\.d
**	fscale	z6\.d, p0/m, z6\.d, \1
**	ret
*/
TEST_DUAL_Z (scale_f64_z_tied2, svfloat64_t, svint64_t,
	     z6_res = svscale_f64_z (p0, z0, z6),
	     z6_res = svscale_z (p0, z0, z6))

/*
** scale_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fscale	z0\.d, p0/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (scale_f64_z_untied, svfloat64_t, svint64_t,
	     z0 = svscale_f64_z (p0, z1, z6),
	     z0 = svscale_z (p0, z1, z6))

/*
** scale_x0_f64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_x0_f64_z_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_z (p0, z0, x0),
		 z0 = svscale_z (p0, z0, x0))

/*
** scale_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_x0_f64_z_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_z (p0, z1, x0),
		 z0 = svscale_z (p0, z1, x0))

/*
** scale_d4_f64_z_tied1:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.d, p0/z, z0\.d
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_d4_f64_z_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_z (p0, z0, d4),
		 z0 = svscale_z (p0, z0, d4))

/*
** scale_d4_f64_z_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.d, p0/z, z1\.d
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_d4_f64_z_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_z (p0, z1, d4),
		 z0 = svscale_z (p0, z1, d4))

/*
** scale_3_f64_z_tied1:
**	mov	(z[0-9]+\.d), #3
**	movprfx	z0\.d, p0/z, z0\.d
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f64_z_tied1, svfloat64_t,
		z0 = svscale_n_f64_z (p0, z0, 3),
		z0 = svscale_z (p0, z0, 3))

/*
** scale_3_f64_z_untied:
**	mov	(z[0-9]+\.d), #3
**	movprfx	z0\.d, p0/z, z1\.d
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f64_z_untied, svfloat64_t,
		z0 = svscale_n_f64_z (p0, z1, 3),
		z0 = svscale_z (p0, z1, 3))

/*
** scale_m3_f64_z:
**	mov	(z[0-9]+\.d), #-3
**	movprfx	z0\.d, p0/z, z0\.d
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_m3_f64_z, svfloat64_t,
		z0 = svscale_n_f64_z (p0, z0, -3),
		z0 = svscale_z (p0, z0, -3))

/*
** scale_f64_x_tied1:
**	fscale	z0\.d, p0/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (scale_f64_x_tied1, svfloat64_t, svint64_t,
	     z0 = svscale_f64_x (p0, z0, z6),
	     z0 = svscale_x (p0, z0, z6))

/*
** scale_f64_x_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	fscale	z6\.d, p0/m, z6\.d, \1
**	ret
*/
TEST_DUAL_Z (scale_f64_x_tied2, svfloat64_t, svint64_t,
	     z6_res = svscale_f64_x (p0, z0, z6),
	     z6_res = svscale_x (p0, z0, z6))

/*
** scale_f64_x_untied:
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (scale_f64_x_untied, svfloat64_t, svint64_t,
	     z0 = svscale_f64_x (p0, z1, z6),
	     z0 = svscale_x (p0, z1, z6))

/*
** scale_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_x0_f64_x_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (p0, z0, x0),
		 z0 = svscale_x (p0, z0, x0))

/*
** scale_x0_f64_x_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_x0_f64_x_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (p0, z1, x0),
		 z0 = svscale_x (p0, z1, x0))

/*
** scale_d4_f64_x_tied1:
**	mov	(z[0-9]+\.d), d4
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_d4_f64_x_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (p0, z0, d4),
		 z0 = svscale_x (p0, z0, d4))

/*
** scale_d4_f64_x_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (scale_d4_f64_x_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (p0, z1, d4),
		 z0 = svscale_x (p0, z1, d4))

/*
** scale_3_f64_x_tied1:
**	mov	(z[0-9]+\.d), #3
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f64_x_tied1, svfloat64_t,
		z0 = svscale_n_f64_x (p0, z0, 3),
		z0 = svscale_x (p0, z0, 3))

/*
** scale_3_f64_x_untied:
**	mov	(z[0-9]+\.d), #3
**	movprfx	z0, z1
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_3_f64_x_untied, svfloat64_t,
		z0 = svscale_n_f64_x (p0, z1, 3),
		z0 = svscale_x (p0, z1, 3))

/*
** scale_m3_f64_x:
**	mov	(z[0-9]+\.d), #-3
**	fscale	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (scale_m3_f64_x, svfloat64_t,
		z0 = svscale_n_f64_x (p0, z0, -3),
		z0 = svscale_x (p0, z0, -3))

/*
** ptrue_scale_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fscale	z0\.d, \1/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_scale_f64_x_tied1, svfloat64_t, svint64_t,
	     z0 = svscale_f64_x (svptrue_b64 (), z0, z6),
	     z0 = svscale_x (svptrue_b64 (), z0, z6))

/*
** ptrue_scale_f64_x_tied2:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	fscale	z6\.d, \1/m, z6\.d, \2
**	ret
*/
TEST_DUAL_Z (ptrue_scale_f64_x_tied2, svfloat64_t, svint64_t,
	     z6_res = svscale_f64_x (svptrue_b64 (), z0, z6),
	     z6_res = svscale_x (svptrue_b64 (), z0, z6))

/*
** ptrue_scale_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fscale	z0\.d, \1/m, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_scale_f64_x_untied, svfloat64_t, svint64_t,
	     z0 = svscale_f64_x (svptrue_b64 (), z1, z6),
	     z0 = svscale_x (svptrue_b64 (), z1, z6))

/*
** ptrue_scale_x0_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), x0
**	fscale	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_x0_f64_x_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (svptrue_b64 (), z0, x0),
		 z0 = svscale_x (svptrue_b64 (), z0, x0))

/*
** ptrue_scale_x0_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fscale	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_x0_f64_x_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (svptrue_b64 (), z1, x0),
		 z0 = svscale_x (svptrue_b64 (), z1, x0))

/*
** ptrue_scale_d4_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), d4
**	fscale	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_d4_f64_x_tied1, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (svptrue_b64 (), z0, d4),
		 z0 = svscale_x (svptrue_b64 (), z0, d4))

/*
** ptrue_scale_d4_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	fscale	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_scale_d4_f64_x_untied, svfloat64_t, int64_t,
		 z0 = svscale_n_f64_x (svptrue_b64 (), z1, d4),
		 z0 = svscale_x (svptrue_b64 (), z1, d4))

/*
** ptrue_scale_3_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), #3
**	fscale	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_scale_3_f64_x_tied1, svfloat64_t,
		z0 = svscale_n_f64_x (svptrue_b64 (), z0, 3),
		z0 = svscale_x (svptrue_b64 (), z0, 3))

/*
** ptrue_scale_3_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), #3
**	movprfx	z0, z1
**	fscale	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_scale_3_f64_x_untied, svfloat64_t,
		z0 = svscale_n_f64_x (svptrue_b64 (), z1, 3),
		z0 = svscale_x (svptrue_b64 (), z1, 3))

/*
** ptrue_scale_m3_f64_x:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), #-3
**	fscale	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_scale_m3_f64_x, svfloat64_t,
		z0 = svscale_n_f64_x (svptrue_b64 (), z0, -3),
		z0 = svscale_x (svptrue_b64 (), z0, -3))
