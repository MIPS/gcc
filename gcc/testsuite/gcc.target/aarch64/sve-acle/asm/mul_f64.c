/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mul_f64_m_tied1:
**	fmul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_m_tied1, svfloat64_t,
		z0 = svmul_f64_m (p0, z0, z1),
		z0 = svmul_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mul_f64_m_tied2, svfloat64_t,
		z1 = svmul_f64_m (p0, z0, z1),
		z1 = svmul_m (p0, z0, z1))

/*
** mul_f64_m_untied:
**	movprfx	z0, z1
**	fmul	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_m_untied, svfloat64_t,
		z0 = svmul_f64_m (p0, z1, z2),
		z0 = svmul_m (p0, z1, z2))

/*
** mul_x0_f64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_f64_m_tied1, svfloat64_t, double,
		 z0 = svmul_n_f64_m (p0, z0, x0),
		 z0 = svmul_m (p0, z0, x0))

/*
** mul_x0_f64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_f64_m_untied, svfloat64_t, double,
		 z0 = svmul_n_f64_m (p0, z1, x0),
		 z0 = svmul_m (p0, z1, x0))

/*
** mul_d0_f64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	fmul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_f64_m_tied1, svfloat64_t, double,
		 z1 = svmul_n_f64_m (p0, z1, d0),
		 z1 = svmul_m (p0, z1, d0))

/*
** mul_d0_f64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	fmul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_f64_m_untied, svfloat64_t, double,
		 z1 = svmul_n_f64_m (p0, z2, d0),
		 z1 = svmul_m (p0, z2, d0))

/*
** mul_1_f64_m_tied1:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f64_m_tied1, svfloat64_t,
		z0 = svmul_n_f64_m (p0, z0, 1),
		z0 = svmul_m (p0, z0, 1))

/*
** mul_1_f64_m_untied:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	movprfx	z0, z1
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f64_m_untied, svfloat64_t,
		z0 = svmul_n_f64_m (p0, z1, 1),
		z0 = svmul_m (p0, z1, 1))

/*
** mul_0p5_f64_m_tied1:
**	fmul	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f64_m_tied1, svfloat64_t,
		z0 = svmul_n_f64_m (p0, z0, 0.5),
		z0 = svmul_m (p0, z0, 0.5))

/*
** mul_0p5_f64_m_untied:
**	movprfx	z0, z1
**	fmul	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f64_m_untied, svfloat64_t,
		z0 = svmul_n_f64_m (p0, z1, 0.5),
		z0 = svmul_m (p0, z1, 0.5))

/*
** mul_2_f64_m_tied1:
**	fmul	z0\.d, p0/m, z0\.d, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f64_m_tied1, svfloat64_t,
		z0 = svmul_n_f64_m (p0, z0, 2.0),
		z0 = svmul_m (p0, z0, 2.0))

/*
** mul_2_f64_m_untied:
**	movprfx	z0, z1
**	fmul	z0\.d, p0/m, z0\.d, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f64_m_untied, svfloat64_t,
		z0 = svmul_n_f64_m (p0, z1, 2.0),
		z0 = svmul_m (p0, z1, 2.0))

/*
** mul_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fmul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_z_tied1, svfloat64_t,
		z0 = svmul_f64_z (p0, z0, z1),
		z0 = svmul_z (p0, z0, z1))

/*
** mul_f64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	fmul	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_z_tied2, svfloat64_t,
		z1 = svmul_f64_z (p0, z0, z1),
		z1 = svmul_z (p0, z0, z1))

/*
** mul_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fmul	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_z_untied, svfloat64_t,
		z0 = svmul_f64_z (p0, z1, z2),
		z0 = svmul_z (p0, z1, z2))

/*
** mul_x0_f64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_f64_z_tied1, svfloat64_t, double,
		 z0 = svmul_n_f64_z (p0, z0, x0),
		 z0 = svmul_z (p0, z0, x0))

/*
** mul_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, \1
**	fmul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_f64_z_untied, svfloat64_t, double,
		 z0 = svmul_n_f64_z (p0, z1, x0),
		 z0 = svmul_z (p0, z1, x0))

/*
** mul_d0_f64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	(z1\.d, p0/z, z1\.d|z1\.d, p0/z, \1)
**	fmul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_f64_z_tied1, svfloat64_t, double,
		 z1 = svmul_n_f64_z (p0, z1, d0),
		 z1 = svmul_z (p0, z1, d0))

/*
** mul_d0_f64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, \1
**	fmul	z1\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_f64_z_untied, svfloat64_t, double,
		 z1 = svmul_n_f64_z (p0, z2, d0),
		 z1 = svmul_z (p0, z2, d0))

/*
** mul_1_f64_z_tied1:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	movprfx	z0\.d, p0/z, z0\.d
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f64_z_tied1, svfloat64_t,
		z0 = svmul_n_f64_z (p0, z0, 1),
		z0 = svmul_z (p0, z0, 1))

/*
** mul_0p5_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fmul	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f64_z_tied1, svfloat64_t,
		z0 = svmul_n_f64_z (p0, z0, 0.5),
		z0 = svmul_z (p0, z0, 0.5))

/*
** mul_0p5_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fmul	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f64_z_untied, svfloat64_t,
		z0 = svmul_n_f64_z (p0, z1, 0.5),
		z0 = svmul_z (p0, z1, 0.5))

/*
** mul_2_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fmul	z0\.d, p0/m, z0\.d, #2.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f64_z_untied, svfloat64_t,
		z0 = svmul_n_f64_z (p0, z1, 2),
		z0 = svmul_z (p0, z1, 2))

/*
** mul_f64_x_tied1:
**	fmul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_x_tied1, svfloat64_t,
		z0 = svmul_f64_x (p0, z0, z1),
		z0 = svmul_x (p0, z0, z1))

/*
** mul_f64_x_tied2:
**	fmul	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_x_tied2, svfloat64_t,
		z1 = svmul_f64_x (p0, z0, z1),
		z1 = svmul_x (p0, z0, z1))

/*
** mul_f64_x_untied:
**	movprfx	z2, z0
**	fmul	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_f64_x_untied, svfloat64_t,
		z2 = svmul_f64_x (p0, z0, z1),
		z2 = svmul_x (p0, z0, z1))

/*
** mul_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svmul_n_f64_x (p0, z0, x0),
		 z0 = svmul_x (p0, z0, x0))

/*
** mul_x0_f64_x_untied:
**	mov	z1\.d, x0
**	fmul	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svmul_n_f64_x (p0, z0, x0),
		 z1 = svmul_x (p0, z0, x0))

/*
** mul_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fmul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svmul_n_f64_x (p0, z1, d0),
		 z1 = svmul_x (p0, z1, d0))

/*
** mul_d0_f64_x_untied:
**	mov	z2\.d, d0
**	fmul	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svmul_n_f64_x (p0, z1, d0),
		 z2 = svmul_x (p0, z1, d0))

/*
** mul_1_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #1\.0(e\+0)?
**	fmul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f64_x_tied1, svfloat64_t,
		z0 = svmul_n_f64_x (p0, z0, 1),
		z0 = svmul_x (p0, z0, 1))

/*
** mul_1_f64_x_untied:
**	fmov	z0\.d, #1\.0(e\+0)?
**	fmul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_1_f64_x_untied, svfloat64_t,
		z0 = svmul_n_f64_x (p0, z1, 1),
		z0 = svmul_x (p0, z1, 1))

/*
** mul_0p5_f64_x_tied1:
**	fmul	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f64_x_tied1, svfloat64_t,
		z0 = svmul_n_f64_x (p0, z0, 0.5),
		z0 = svmul_x (p0, z0, 0.5))

/*
** mul_0p5_f64_x_untied:
**	movprfx	z0, z1
**	fmul	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f64_x_untied, svfloat64_t,
		z0 = svmul_n_f64_x (p0, z1, 0.5),
		z0 = svmul_x (p0, z1, 0.5))

/*
** mul_2_f64_x_tied1:
**	fmul	z0\.d, p0/m, z0\.d, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f64_x_tied1, svfloat64_t,
		z0 = svmul_n_f64_x (p0, z0, 2),
		z0 = svmul_x (p0, z0, 2))

/*
** mul_2_f64_x_untied:
**	movprfx	z0, z1
**	fmul	z0\.d, p0/m, z0\.d, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f64_x_untied, svfloat64_t,
		z0 = svmul_n_f64_x (p0, z1, 2),
		z0 = svmul_x (p0, z1, 2))

/*
** ptrue_mul_f64_x_tied1:
**	fmul	(z0\.d, z1\.d, z0\.d|z0\.d, z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_f64_x_tied1, svfloat64_t,
		z0 = svmul_f64_x (svptrue_b64 (), z0, z1),
		z0 = svmul_x (svptrue_b64 (), z0, z1))

/*
** ptrue_mul_f64_x_tied2:
**	fmul	(z1\.d, z1\.d, z0\.d|z1\.d, z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_f64_x_tied2, svfloat64_t,
		z1 = svmul_f64_x (svptrue_b64 (), z0, z1),
		z1 = svmul_x (svptrue_b64 (), z0, z1))

/*
** ptrue_mul_f64_x_untied:
**	fmul	(z2\.d, z1\.d, z0\.d|z2\.d, z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_f64_x_untied, svfloat64_t,
		z2 = svmul_f64_x (svptrue_b64 (), z0, z1),
		z2 = svmul_x (svptrue_b64 (), z0, z1))

/*
** ptrue_mul_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fmul	(z0\.d, \1, z0\.d|z0\.d, z0\.d, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svmul_n_f64_x (svptrue_b64 (), z0, x0),
		 z0 = svmul_x (svptrue_b64 (), z0, x0))

/*
** ptrue_mul_x0_f64_x_untied:
**	mov	(z[0-9]+\.d), x0
**	fmul	(z1\.d, \1, z0\.d|z1\.d, z0\.d, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svmul_n_f64_x (svptrue_b64 (), z0, x0),
		 z1 = svmul_x (svptrue_b64 (), z0, x0))

/*
** ptrue_mul_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fmul	(z1\.d, \1, z1\.d|z1\.d, z1\.d, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svmul_n_f64_x (svptrue_b64 (), z1, d0),
		 z1 = svmul_x (svptrue_b64 (), z1, d0))

/*
** ptrue_mul_d0_f64_x_untied:
**	mov	(z[0-9]+\.d), d0
**	fmul	(z2\.d, \1, z1\.d|z2\.d, z1\.d, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svmul_n_f64_x (svptrue_b64 (), z1, d0),
		 z2 = svmul_x (svptrue_b64 (), z1, d0))

/*
** ptrue_mul_1_f64_x_untied:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	fmul	(z0\.d, \1, z1\.d|z0\.d, z1\.d, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_1_f64_x_untied, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z1, 1),
		z0 = svmul_x (svptrue_b64 (), z1, 1))

/*
** ptrue_mul_0p5_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fmul	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_0p5_f64_x_tied1, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z0, 0.5),
		z0 = svmul_x (svptrue_b64 (), z0, 0.5))

/*
** ptrue_mul_0p5_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fmul	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_0p5_f64_x_untied, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z1, 0.5),
		z0 = svmul_x (svptrue_b64 (), z1, 0.5))

/*
** ptrue_mul_m1_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #-1.0(e\+0)?
**	fmul	(z0\.d, \1, z0\.d|z0\.d, z0\.d, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m1_f64_x_tied1, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z0, -1),
		z0 = svmul_x (svptrue_b64 (), z0, -1))

/*
** ptrue_mul_m1_f64_x_untied:
**	fmov	(z[0-9]+\.d), #-1.0(e\+0)?
**	fmul	(z0\.d, \1, z1\.d|z0\.d, z1\.d, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m1_f64_x_untied, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z1, -1),
		z0 = svmul_x (svptrue_b64 (), z1, -1))

/*
** ptrue_mul_m0p5_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #-5.0(e\-1)?
**	fmul	(z0\.d, \1, z0\.d|z0\.d, z0\.d, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m0p5_f64_x_tied1, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z0, -0.5),
		z0 = svmul_x (svptrue_b64 (), z0, -0.5))

/*
** ptrue_mul_m0p5_f64_x_untied:
**	fmov	(z[0-9]+\.d), #-5.0(e\-1)?
**	fmul	(z0\.d, \1, z1\.d|z0\.d, z1\.d, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m0p5_f64_x_untied, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z1, -0.5),
		z0 = svmul_x (svptrue_b64 (), z1, -0.5))

/*
** ptrue_mul_2_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fmul	z0\.d, \1/m, z0\.d, #2.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_2_f64_x_tied1, svfloat64_t,
		z0 = svmul_n_f64_x (svptrue_b64 (), z0, 2),
		z0 = svmul_x (svptrue_b64 (), z0, 2))

/*
** ptrue_mul_2_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z1, z0
**	fmul	z1\.d, \1/m, z1\.d, #2.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_2_f64_x_untied, svfloat64_t,
		z1 = svmul_n_f64_x (svptrue_b64 (), z0, 2),
		z1 = svmul_x (svptrue_b64 (), z0, 2))

/*
** ptrue_b8_mul_f64_x_tied1:
**	fmul	(z0\.d, z1\.d, z0\.d|z0\.d, z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_mul_f64_x_tied1, svfloat64_t,
		z0 = svmul_f64_x (svptrue_b8 (), z0, z1),
		z0 = svmul_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_mul_f64_x_tied2:
**	fmul	(z1\.d, z1\.d, z0\.d|z1\.d, z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_mul_f64_x_tied2, svfloat64_t,
		z1 = svmul_f64_x (svptrue_b8 (), z0, z1),
		z1 = svmul_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_mul_f64_x_untied:
**	fmul	(z2\.d, z1\.d, z0\.d|z2\.d, z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_mul_f64_x_untied, svfloat64_t,
		z2 = svmul_f64_x (svptrue_b8 (), z0, z1),
		z2 = svmul_x (svptrue_b8 (), z0, z1))
