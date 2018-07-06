/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** add_f64_m:
**	fadd	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_m, svfloat64_t,
		z0 = svadd_f64_m (p0, z0, z1),
		z0 = svadd_m (p0, z0, z1))

/*
** add_f64_m_prfx:
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_m_prfx, svfloat64_t,
		z0 = svadd_f64_m (p0, z1, z2),
		z0 = svadd_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (add_f64_m_bad_ra, svfloat64_t,
		z1 = svadd_f64_m (p0, z0, z1),
		z1 = svadd_m (p0, z0, z1))

/*
** add_x0_f64_m:
**	mov	(z[0-9]+\.d), x0
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_m, svfloat64_t, double,
		 z0 = svadd_n_f64_m (p0, z0, x0),
		 z0 = svadd_m (p0, z0, x0))

/*
** add_x0_f64_m_prfx:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_m_prfx, svfloat64_t, double,
		 z0 = svadd_n_f64_m (p0, z1, x0),
		 z0 = svadd_m (p0, z1, x0))

/*
** add_d0_f64_m:
**	mov	(z[0-9]+\.d), d0
**	fadd	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_m, svfloat64_t, double,
		 z1 = svadd_n_f64_m (p0, z1, d0),
		 z1 = svadd_m (p0, z1, d0))

/*
** add_d0_f64_m_prfx:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	fadd	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_m_prfx, svfloat64_t, double,
		 z1 = svadd_n_f64_m (p0, z2, d0),
		 z1 = svadd_m (p0, z2, d0))

/*
** add_1_f64_m:
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_m, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, 1),
		z0 = svadd_m (p0, z0, 1))

/*
** add_1_f64_m_prfx:
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_m_prfx, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, 1),
		z0 = svadd_m (p0, z1, 1))

/*
** add_0p5_f64_m:
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_m, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, 0.5),
		z0 = svadd_m (p0, z0, 0.5))

/*
** add_0p5_f64_m_prfx:
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_m_prfx, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, 0.5),
		z0 = svadd_m (p0, z1, 0.5))

/*
** add_m1_f64_m:
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_m, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, -1),
		z0 = svadd_m (p0, z0, -1))

/*
** add_m1_f64_m_prfx:
**	movprfx	z0, z1
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_m_prfx, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, -1),
		z0 = svadd_m (p0, z1, -1))

/*
** add_m0p5_f64_m:
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_m, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, -0.5),
		z0 = svadd_m (p0, z0, -0.5))

/*
** add_m0p5_f64_m_prfx:
**	movprfx	z0, z1
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_m_prfx, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z1, -0.5),
		z0 = svadd_m (p0, z1, -0.5))

/*
** add_m2_f64_m:
**	fmov	(z[0-9]+\.d), #-2\.0(e\+0)?
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m2_f64_m, svfloat64_t,
		z0 = svadd_n_f64_m (p0, z0, -2),
		z0 = svadd_m (p0, z0, -2))

/*
** add_f64_z_tied:
**	movprfx	z0\.d, p0/z, z0\.d
**	fadd	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_z_tied, svfloat64_t,
		z0 = svadd_f64_z (p0, z0, z1),
		z0 = svadd_z (p0, z0, z1))

/*
** add_f64_z_rev:
**	movprfx	z1\.d, p0/z, z1\.d
**	fadd	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_z_rev, svfloat64_t,
		z1 = svadd_f64_z (p0, z0, z1),
		z1 = svadd_z (p0, z0, z1))

/*
** add_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fadd	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_z_untied, svfloat64_t,
		z0 = svadd_f64_z (p0, z1, z2),
		z0 = svadd_z (p0, z1, z2))

/*
** add_x0_f64_z_tied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_z_tied, svfloat64_t, double,
		 z0 = svadd_n_f64_z (p0, z0, x0),
		 z0 = svadd_z (p0, z0, x0))

/*
** add_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_z_untied, svfloat64_t, double,
		 z0 = svadd_n_f64_z (p0, z1, x0),
		 z0 = svadd_z (p0, z1, x0))

/*
** add_d0_f64_z_tied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	fadd	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_z_tied, svfloat64_t, double,
		 z1 = svadd_n_f64_z (p0, z1, d0),
		 z1 = svadd_z (p0, z1, d0))

/*
** add_1_f64_z_tied:
**	movprfx	z0\.d, p0/z, z0\.d
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_z_tied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, 1),
		z0 = svadd_z (p0, z0, 1))

/*
** add_1_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, 1),
		z0 = svadd_z (p0, z1, 1))

/*
** add_0p5_f64_z_tied:
**	movprfx	z0\.d, p0/z, z0\.d
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_z_tied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, 0.5),
		z0 = svadd_z (p0, z0, 0.5))

/*
** add_0p5_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, 0.5),
		z0 = svadd_z (p0, z1, 0.5))

/*
** add_m1_f64_z_tied:
**	movprfx	z0\.d, p0/z, z0\.d
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_z_tied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, -1),
		z0 = svadd_z (p0, z0, -1))

/*
** add_m1_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, -1),
		z0 = svadd_z (p0, z1, -1))

/*
** add_m0p5_f64_z_tied:
**	movprfx	z0\.d, p0/z, z0\.d
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_z_tied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, -0.5),
		z0 = svadd_z (p0, z0, -0.5))

/*
** add_m0p5_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_z_untied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z1, -0.5),
		z0 = svadd_z (p0, z1, -0.5))

/*
** add_m2_f64_z_tied:
**	fmov	(z[0-9]+\.d), #-2\.0(e\+0)?
**	movprfx	z0\.d, p0/z, z0\.d
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m2_f64_z_tied, svfloat64_t,
		z0 = svadd_n_f64_z (p0, z0, -2),
		z0 = svadd_z (p0, z0, -2))

/*
** add_f64_x_z0:
**	fadd	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_x_z0, svfloat64_t,
		z0 = svadd_f64_x (p0, z0, z1),
		z0 = svadd_x (p0, z0, z1))

/*
** add_f64_x_z1:
**	fadd	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_x_z1, svfloat64_t,
		z1 = svadd_f64_x (p0, z0, z1),
		z1 = svadd_x (p0, z0, z1))

/*
** add_f64_x_z2:
**	movprfx	z2, z0
**	fadd	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_f64_x_z2, svfloat64_t,
		z2 = svadd_f64_x (p0, z0, z1),
		z2 = svadd_x (p0, z0, z1))

/*
** add_x0_f64_x_z0:
**	mov	(z[0-9]+\.d), x0
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_x_z0, svfloat64_t, double,
		 z0 = svadd_n_f64_x (p0, z0, x0),
		 z0 = svadd_x (p0, z0, x0))

/*
** add_x0_f64_x_z1:
**	mov	z1\.d, x0
**	fadd	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (add_x0_f64_x_z1, svfloat64_t, double,
		 z1 = svadd_n_f64_x (p0, z0, x0),
		 z1 = svadd_x (p0, z0, x0))

/*
** add_d0_f64_x_z1:
**	mov	(z[0-9]+\.d), d0
**	fadd	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_x_z1, svfloat64_t, double,
		 z1 = svadd_n_f64_x (p0, z1, d0),
		 z1 = svadd_x (p0, z1, d0))

/*
** add_d0_f64_x_z2:
**	mov	z2\.d, d0
**	fadd	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (add_d0_f64_x_z2, svfloat64_t, double,
		 z2 = svadd_n_f64_x (p0, z1, d0),
		 z2 = svadd_x (p0, z1, d0))

/*
** add_1_f64_x:
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, 1),
		z0 = svadd_x (p0, z0, 1))

/*
** add_1_f64_x_prfx:
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_1_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, 1),
		z0 = svadd_x (p0, z1, 1))

/*
** add_0p5_f64_x:
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, 0.5),
		z0 = svadd_x (p0, z0, 0.5))

/*
** add_0p5_f64_x_prfx:
**	movprfx	z0, z1
**	fadd	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_0p5_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, 0.5),
		z0 = svadd_x (p0, z1, 0.5))

/*
** add_m1_f64_x:
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, -1),
		z0 = svadd_x (p0, z0, -1))

/*
** add_m1_f64_x_prfx:
**	movprfx	z0, z1
**	fsub	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (add_m1_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, -1),
		z0 = svadd_x (p0, z1, -1))

/*
** add_m0p5_f64_x:
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, -0.5),
		z0 = svadd_x (p0, z0, -0.5))

/*
** add_m0p5_f64_x_prfx:
**	movprfx	z0, z1
**	fsub	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (add_m0p5_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, -0.5),
		z0 = svadd_x (p0, z1, -0.5))

/*
** add_2_f64_x_tied:
**	fmov	(z[0-9]+\.d), #2\.0(e\+0)?
**	fadd	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_2_f64_x_tied, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z0, 2),
		z0 = svadd_x (p0, z0, 2))

/*
** add_2_f64_x_untied:
**	fmov	z0\.d, #2\.0(e\+0)?
**	fadd	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_2_f64_x_untied, svfloat64_t,
		z0 = svadd_n_f64_x (p0, z1, 2),
		z0 = svadd_x (p0, z1, 2))

/*
** ptrue_add_f64_x_z0:
**	fadd	z0\.d, (z0\.d, z1\.d||z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_f64_x_z0, svfloat64_t,
		z0 = svadd_f64_x (svptrue_b64 (), z0, z1),
		z0 = svadd_x (svptrue_b64 (), z0, z1))

/*
** ptrue_add_f64_x_z1:
**	fadd	z1\.d, (z1\.d, z0\.d|z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_f64_x_z1, svfloat64_t,
		z1 = svadd_f64_x (svptrue_b64 (), z0, z1),
		z1 = svadd_x (svptrue_b64 (), z0, z1))

/*
** ptrue_add_f64_x_z2:
**	fadd	z2\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_f64_x_z2, svfloat64_t,
		z2 = svadd_f64_x (svptrue_b64 (), z0, z1),
		z2 = svadd_x (svptrue_b64 (), z0, z1))

/*
** ptrue_add_x0_f64_x_z0:
**	mov	(z[0-9]+\.d), x0
**	fadd	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_x0_f64_x_z0, svfloat64_t, double,
		 z0 = svadd_n_f64_x (svptrue_b64 (), z0, x0),
		 z0 = svadd_x (svptrue_b64 (), z0, x0))

/*
** ptrue_add_x0_f64_x_z1:
**	mov	(z[0-9]+\.d), x0
**	fadd	z1\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_x0_f64_x_z1, svfloat64_t, double,
		 z1 = svadd_n_f64_x (svptrue_b64 (), z0, x0),
		 z1 = svadd_x (svptrue_b64 (), z0, x0))

/*
** ptrue_add_d0_f64_x_z1:
**	mov	(z[0-9]+\.d), d0
**	fadd	z1\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_d0_f64_x_z1, svfloat64_t, double,
		 z1 = svadd_n_f64_x (svptrue_b64 (), z1, d0),
		 z1 = svadd_x (svptrue_b64 (), z1, d0))

/*
** ptrue_add_d0_f64_x_z2:
**	mov	(z[0-9]+\.d), d0
**	fadd	z2\.d, (z1\.d, \1|\1, z1\.d)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_add_d0_f64_x_z2, svfloat64_t, double,
		 z2 = svadd_n_f64_x (svptrue_b64 (), z1, d0),
		 z2 = svadd_x (svptrue_b64 (), z1, d0))

/*
** ptrue_add_1_f64_x:
**	ptrue	(p[0-7])\.d[^\n]*
**	fadd	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_1_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, 1),
		z0 = svadd_x (svptrue_b64 (), z0, 1))

/*
** ptrue_add_1_f64_x_prfx:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_1_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, 1),
		z0 = svadd_x (svptrue_b64 (), z1, 1))

/*
** ptrue_add_0p5_f64_x:
**	ptrue	(p[0-7])\.d[^\n]*
**	fadd	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_0p5_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, 0.5),
		z0 = svadd_x (svptrue_b64 (), z0, 0.5))

/*
** ptrue_add_0p5_f64_x_prfx:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_0p5_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, 0.5),
		z0 = svadd_x (svptrue_b64 (), z1, 0.5))

/*
** ptrue_add_m1_f64_x:
**	ptrue	(p[0-7])\.d[^\n]*
**	fsub	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m1_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, -1),
		z0 = svadd_x (svptrue_b64 (), z0, -1))

/*
** ptrue_add_m1_f64_x_prfx:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m1_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, -1),
		z0 = svadd_x (svptrue_b64 (), z1, -1))

/*
** ptrue_add_m0p5_f64_x:
**	ptrue	(p[0-7])\.d[^\n]*
**	fsub	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m0p5_f64_x, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, -0.5),
		z0 = svadd_x (svptrue_b64 (), z0, -0.5))

/*
** ptrue_add_m0p5_f64_x_prfx:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_m0p5_f64_x_prfx, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z1, -0.5),
		z0 = svadd_x (svptrue_b64 (), z1, -0.5))

/*
** ptrue_add_2_f64_x_z0:
**	fmov	(z[0-9]+\.d), #2\.0(e\+0)?
**	fadd	z0\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_2_f64_x_z0, svfloat64_t,
		z0 = svadd_n_f64_x (svptrue_b64 (), z0, 2),
		z0 = svadd_x (svptrue_b64 (), z0, 2))

/*
** ptrue_add_2_f64_x_z1:
**	fmov	(z[0-9]+\.d), #2\.0(e\+0)?
**	fadd	z1\.d, (z0\.d, \1|\1, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_add_2_f64_x_z1, svfloat64_t,
		z1 = svadd_n_f64_x (svptrue_b64 (), z0, 2),
		z1 = svadd_x (svptrue_b64 (), z0, 2))

/*
** ptrue_b32_add_f64_x_z0:
**	fadd	z0\.d, (z0\.d, z1\.d||z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b32_add_f64_x_z0, svfloat64_t,
		z0 = svadd_f64_x (svptrue_b32 (), z0, z1),
		z0 = svadd_x (svptrue_b32 (), z0, z1))

/*
** ptrue_b32_add_f64_x_z1:
**	fadd	z1\.d, (z1\.d, z0\.d|z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b32_add_f64_x_z1, svfloat64_t,
		z1 = svadd_f64_x (svptrue_b32 (), z0, z1),
		z1 = svadd_x (svptrue_b32 (), z0, z1))

/*
** ptrue_b32_add_f64_x_z2:
**	fadd	z2\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b32_add_f64_x_z2, svfloat64_t,
		z2 = svadd_f64_x (svptrue_b32 (), z0, z1),
		z2 = svadd_x (svptrue_b32 (), z0, z1))

/*
** ptrue_b16_add_f64_x_z0:
**	fadd	z0\.d, (z0\.d, z1\.d||z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_add_f64_x_z0, svfloat64_t,
		z0 = svadd_f64_x (svptrue_b16 (), z0, z1),
		z0 = svadd_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_add_f64_x_z1:
**	fadd	z1\.d, (z1\.d, z0\.d|z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_add_f64_x_z1, svfloat64_t,
		z1 = svadd_f64_x (svptrue_b16 (), z0, z1),
		z1 = svadd_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_add_f64_x_z2:
**	fadd	z2\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_add_f64_x_z2, svfloat64_t,
		z2 = svadd_f64_x (svptrue_b16 (), z0, z1),
		z2 = svadd_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b8_add_f64_x_z0:
**	fadd	z0\.d, (z0\.d, z1\.d||z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_add_f64_x_z0, svfloat64_t,
		z0 = svadd_f64_x (svptrue_b8 (), z0, z1),
		z0 = svadd_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_add_f64_x_z1:
**	fadd	z1\.d, (z1\.d, z0\.d|z0\.d, z1\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_add_f64_x_z1, svfloat64_t,
		z1 = svadd_f64_x (svptrue_b8 (), z0, z1),
		z1 = svadd_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_add_f64_x_z2:
**	fadd	z2\.d, (z0\.d, z1\.d|z1\.d, z0\.d)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_add_f64_x_z2, svfloat64_t,
		z2 = svadd_f64_x (svptrue_b8 (), z0, z1),
		z2 = svadd_x (svptrue_b8 (), z0, z1))
