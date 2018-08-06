/* { dg-do compile } */
/* { dg-additional-options "-fno-trapping-math" } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** add_f64_m_tied1:
**	fadd	z0.d, p0/m, z0.d, z1.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_m_tied1, svfloat64_t,
		z0 = svadd_f64_m (p0, z0, z1),
		z0 = svadd_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (add_f64_m_tied2, svfloat64_t,
		z1 = svadd_f64_m (p0, z0, z1),
		z1 = svadd_m (p0, z0, z1))

/*
** add_f64_m_untied:
**	movprfx	z0, z1
**	fadd	z0.d, p0/m, z0.d, z2.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_m_untied, svfloat64_t,
		z0 = svadd_f64_m (p0, z1, z2),
		z0 = svadd_m (p0, z1, z2))

/*
** add_x0_f64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	fadd	z0.d, p0/m, z0.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_m_tied1, svfloat64_t, double,
		 z0 = svadd_n_f64_m (p0, z0, x0),
		 z0 = svadd_m (p0, z0, x0))

/*
** add_x0_f64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fadd	z0.d, p0/m, z0.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_m_untied, svfloat64_t, double,
		 z0 = svadd_n_f64_m (p0, z1, x0),
		 z0 = svadd_m (p0, z1, x0))

/*
** add_d0_f64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	fadd	z1.d, p0/m, z1.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_m_tied1, svfloat64_t, double,
		 z1 = svadd_n_f64_m (p0, z1, d0),
		 z1 = svadd_m (p0, z1, d0))

/*
** add_d0_f64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	fadd	z1.d, p0/m, z1.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_m_untied, svfloat64_t, double,
		 z1 = svadd_n_f64_m (p0, z2, d0),
		 z1 = svadd_m (p0, z2, d0))

/*
** add_1_f64_m_tied1:
**	fadd	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_m_tied1, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, 1),
		z0 = svadd_m (p0, z0, 1))

/*
** add_1_f64_m_untied:
**	movprfx	z0, z1
**	fadd	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_m_untied, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, 1),
		z0 = svadd_m (p0, z1, 1))

/*
** add_0p5_f64_m_tied1:
**	fadd	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_m_tied1, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, 0.5),
		z0 = svadd_m (p0, z0, 0.5))

/*
** add_0p5_f64_m_untied:
**	movprfx	z0, z1
**	fadd	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_m_untied, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, 0.5),
		z0 = svadd_m (p0, z1, 0.5))

/*
** add_m1_f64_m_tied1:
**	fsub	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_m_tied1, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, -1),
		z0 = svadd_m (p0, z0, -1))

/*
** add_m1_f64_m_untied:
**	movprfx	z0, z1
**	fsub	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_m_untied, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, -1),
		z0 = svadd_m (p0, z1, -1))

/*
** add_m0p5_f64_m_tied1:
**	fsub	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_m_tied1, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, -0.5),
		z0 = svadd_m (p0, z0, -0.5))

/*
** add_m0p5_f64_m_untied:
**	movprfx	z0, z1
**	fsub	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_m_untied, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, -0.5),
		z0 = svadd_m (p0, z1, -0.5))

/*
** add_m2_f64_m:
**	fmov	(z[0-9]+\.d), #-2\.0(e\+0)?
**	fadd	z0.d, p0/m, z0.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m2_f64_m, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, -2),
		z0 = svadd_m (p0, z0, -2))

/*
** add_f64_z_tied1:
**	movprfx	z0.d, p0/z, z0.d
**	fadd	z0.d, p0/m, z0.d, z1.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_z_tied1, svfloat64_t,
		z0 = svadd_f64_z (p0, z0, z1),
		z0 = svadd_z (p0, z0, z1))

/*
** add_f64_z_tied2:
**	movprfx	z1.d, p0/z, z1.d
**	fadd	z1.d, p0/m, z1.d, z0.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_z_tied2, svfloat64_t,
		z1 = svadd_f64_z (p0, z0, z1),
		z1 = svadd_z (p0, z0, z1))

/*
** add_f64_z_untied:
**	movprfx	z0.d, p0/z, z1.d
**	fadd	z0.d, p0/m, z0.d, z2.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_z_untied, svfloat64_t,
		z0 = svadd_f64_z (p0, z1, z2),
		z0 = svadd_z (p0, z1, z2))

/*
** add_x0_f64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0.d, p0/z, z0.d
**	fadd	z0.d, p0/m, z0.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_z_tied1, svfloat64_t, double,
		 z0 = svadd_n_f64_z (p0, z0, x0),
		 z0 = svadd_z (p0, z0, x0))

/*
** add_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0.d, p0/z, z1.d
**	fadd	z0.d, p0/m, z0.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_z_untied, svfloat64_t, double,
		 z0 = svadd_n_f64_z (p0, z1, x0),
		 z0 = svadd_z (p0, z1, x0))

/*
** add_d0_f64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1.d, p0/z, z1.d
**	fadd	z1.d, p0/m, z1.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_z_tied1, svfloat64_t, double,
		 z1 = svadd_n_f64_z (p0, z1, d0),
		 z1 = svadd_z (p0, z1, d0))

/*
** add_d0_f64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z2\.d
**	fadd	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_z_untied, svfloat64_t, double,
		 z1 = svadd_n_f64_z (p0, z2, d0),
		 z1 = svadd_z (p0, z2, d0))

/*
** add_1_f64_z_tied1:
**	movprfx	z0.d, p0/z, z0.d
**	fadd	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_z_tied1, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, 1),
		z0 = svadd_z (p0, z0, 1))

/*
** add_1_f64_z_untied:
**	movprfx	z0.d, p0/z, z1.d
**	fadd	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, 1),
		z0 = svadd_z (p0, z1, 1))

/*
** add_0p5_f64_z_tied1:
**	movprfx	z0.d, p0/z, z0.d
**	fadd	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_z_tied1, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, 0.5),
		z0 = svadd_z (p0, z0, 0.5))

/*
** add_0p5_f64_z_untied:
**	movprfx	z0.d, p0/z, z1.d
**	fadd	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, 0.5),
		z0 = svadd_z (p0, z1, 0.5))

/*
** add_m1_f64_z_tied1:
**	movprfx	z0.d, p0/z, z0.d
**	fsub	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_z_tied1, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, -1),
		z0 = svadd_z (p0, z0, -1))

/*
** add_m1_f64_z_untied:
**	movprfx	z0.d, p0/z, z1.d
**	fsub	z0.d, p0/m, z0.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, -1),
		z0 = svadd_z (p0, z1, -1))

/*
** add_m0p5_f64_z_tied1:
**	movprfx	z0.d, p0/z, z0.d
**	fsub	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_z_tied1, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, -0.5),
		z0 = svadd_z (p0, z0, -0.5))

/*
** add_m0p5_f64_z_untied:
**	movprfx	z0.d, p0/z, z1.d
**	fsub	z0.d, p0/m, z0.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, -0.5),
		z0 = svadd_z (p0, z1, -0.5))

/*
** add_m2_f64_z:
**	fmov	(z[0-9]+\.d), #-2\.0(e\+0)?
**	movprfx	z0.d, p0/z, z0.d
**	fadd	z0.d, p0/m, z0.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m2_f64_z, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, -2),
		z0 = svadd_z (p0, z0, -2))

/*
** add_f64_x_tied1:
**	fadd	z0\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (add_f64_x_tied1, svfloat64_t,
		z0 = svadd_f64_x (p0, z0, z1),
		z0 = svadd_x (p0, z0, z1))

/*
** add_f64_x_tied2:
**	fadd	z1\.d, (z1\.d, z0\.d|z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (add_f64_x_tied2, svfloat64_t,
		z1 = svadd_f64_x (p0, z0, z1),
		z1 = svadd_x (p0, z0, z1))

/*
** add_f64_x_untied:
**	fadd	z2\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (add_f64_x_untied, svfloat64_t,
		z2 = svadd_f64_x (p0, z0, z1),
		z2 = svadd_x (p0, z0, z1))

/*
** add_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fadd	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svadd_n_f64_x (p0, z0, x0),
		 z0 = svadd_x (p0, z0, x0))

/*
** add_x0_f64_x_untied:
**	mov	(z[0-9]+\.d), x0
**	fadd	z1\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svadd_n_f64_x (p0, z0, x0),
		 z1 = svadd_x (p0, z0, x0))

/*
** add_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fadd	z1\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svadd_n_f64_x (p0, z1, d0),
		 z1 = svadd_x (p0, z1, d0))

/*
** add_d0_f64_x_untied:
**	mov	(z[0-9]+\.d), d0
**	fadd	z2\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svadd_n_f64_x (p0, z1, d0),
		 z2 = svadd_x (p0, z1, d0))

/*
** add_1_f64_x_tied1:
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, 1),
		z0 = svadd_x (p0, z0, 1))

/*
** add_1_f64_x_untied:
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, 1),
		z0 = svadd_x (p0, z1, 1))

/*
** add_0p5_f64_x_tied1:
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, 0.5),
		z0 = svadd_x (p0, z0, 0.5))

/*
** add_0p5_f64_x_untied:
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, 0.5),
		z0 = svadd_x (p0, z1, 0.5))

/*
** add_m1_f64_x_tied1:
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, -1),
		z0 = svadd_x (p0, z0, -1))

/*
** add_m1_f64_x_untied:
**	movprfx	z0, z1
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, -1),
		z0 = svadd_x (p0, z1, -1))

/*
** add_m0p5_f64_x_tied1:
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, -0.5),
		z0 = svadd_x (p0, z0, -0.5))

/*
** add_m0p5_f64_x_untied:
**	movprfx	z0, z1
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, -0.5),
		z0 = svadd_x (p0, z1, -0.5))

/*
** add_2_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #2\.0(e\+0)?
**	fadd	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (add_2_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, 2),
		z0 = svadd_x (p0, z0, 2))

/*
** add_2_f64_x_untied:
**	fmov	(z[0-9]+\.d), #2\.0(e\+0)?
**	fadd	z0\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (add_2_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, 2),
		z0 = svadd_x (p0, z1, 2))

/*
** ptrue_add_f64_x_tied1:
**	fadd	z0\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_f64_x_tied1, svfloat64_t,
		z0 = svadd_f64_x (svptrue_b64 (), z0, z1),
		z0 = svadd_x (svptrue_b64 (), z0, z1))

/*
** ptrue_add_f64_x_tied2:
**	fadd	z1\.d, (z1\.d, z0\.d|z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_f64_x_tied2, svfloat64_t,
		z1 = svadd_f64_x (svptrue_b64 (), z0, z1),
		z1 = svadd_x (svptrue_b64 (), z0, z1))

/*
** ptrue_add_f64_x_untied:
**	fadd	z2\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_f64_x_untied, svfloat64_t,
		z2 = svadd_f64_x (svptrue_b64 (), z0, z1),
		z2 = svadd_x (svptrue_b64 (), z0, z1))

/*
** ptrue_add_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fadd	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svadd_n_f64_x (svptrue_b64 (), z0, x0),
		 z0 = svadd_x (svptrue_b64 (), z0, x0))

/*
** ptrue_add_x0_f64_x_untied:
**	mov	(z[0-9]+\.d), x0
**	fadd	z1\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svadd_n_f64_x (svptrue_b64 (), z0, x0),
		 z1 = svadd_x (svptrue_b64 (), z0, x0))

/*
** ptrue_add_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fadd	z1\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svadd_n_f64_x (svptrue_b64 (), z1, d0),
		 z1 = svadd_x (svptrue_b64 (), z1, d0))

/*
** ptrue_add_d0_f64_x_untied:
**	mov	(z[0-9]+\.d), d0
**	fadd	z2\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svadd_n_f64_x (svptrue_b64 (), z1, d0),
		 z2 = svadd_x (svptrue_b64 (), z1, d0))

/*
** ptrue_add_1_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fadd	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_1_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, 1),
		z0 = svadd_x (svptrue_b64 (), z0, 1))

/*
** ptrue_add_1_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_1_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, 1),
		z0 = svadd_x (svptrue_b64 (), z1, 1))

/*
** ptrue_add_0p5_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fadd	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_0p5_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, 0.5),
		z0 = svadd_x (svptrue_b64 (), z0, 0.5))

/*
** ptrue_add_0p5_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_0p5_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, 0.5),
		z0 = svadd_x (svptrue_b64 (), z1, 0.5))

/*
** ptrue_add_m1_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fsub	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m1_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, -1),
		z0 = svadd_x (svptrue_b64 (), z0, -1))

/*
** ptrue_add_m1_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m1_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, -1),
		z0 = svadd_x (svptrue_b64 (), z1, -1))

/*
** ptrue_add_m0p5_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fsub	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m0p5_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, -0.5),
		z0 = svadd_x (svptrue_b64 (), z0, -0.5))

/*
** ptrue_add_m0p5_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m0p5_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, -0.5),
		z0 = svadd_x (svptrue_b64 (), z1, -0.5))

/*
** ptrue_add_2_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #2\.0(e\+0)?
**	fadd	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_2_f64_x_tied1, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, 2),
		z0 = svadd_x (svptrue_b64 (), z0, 2))

/*
** ptrue_add_2_f64_x_untied:
**	fmov	(z[0-9]+\.d), #2\.0(e\+0)?
**	fadd	z0\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_2_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, 2),
		z0 = svadd_x (svptrue_b64 (), z1, 2))
