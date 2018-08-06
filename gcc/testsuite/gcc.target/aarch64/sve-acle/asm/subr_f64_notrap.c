/* { dg-do compile } */
/* { dg-additional-options "-fno-trapping-math" } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** subr_f64_m_tied1:
**	fsubr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_m_tied1, svfloat64_t,
		z0 = svsubr_f64_m (p0, z0, z1),
		z0 = svsubr_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (subr_f64_m_tied2, svfloat64_t,
		z1 = svsubr_f64_m (p0, z0, z1),
		z1 = svsubr_m (p0, z0, z1))

/*
** subr_f64_m_untied:
**	movprfx	z0, z1
**	fsubr	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_m_untied, svfloat64_t,
		z0 = svsubr_f64_m (p0, z1, z2),
		z0 = svsubr_m (p0, z1, z2))

/*
** subr_x0_f64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	fsubr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_x0_f64_m_tied1, svfloat64_t, double,
		 z0 = svsubr_n_f64_m (p0, z0, x0),
		 z0 = svsubr_m (p0, z0, x0))

/*
** subr_x0_f64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fsubr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_x0_f64_m_untied, svfloat64_t, double,
		 z0 = svsubr_n_f64_m (p0, z1, x0),
		 z0 = svsubr_m (p0, z1, x0))

/*
** subr_d0_f64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	fsubr	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_d0_f64_m_tied1, svfloat64_t, double,
		 z1 = svsubr_n_f64_m (p0, z1, d0),
		 z1 = svsubr_m (p0, z1, d0))

/*
** subr_d0_f64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	fsubr	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_d0_f64_m_untied, svfloat64_t, double,
		 z1 = svsubr_n_f64_m (p0, z2, d0),
		 z1 = svsubr_m (p0, z2, d0))

/*
** subr_1_f64_m_tied1:
**	fsubr	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f64_m_tied1, svfloat64_t,
		z0 = svsubr_n_f64_m (p0, z0, 1),
		z0 = svsubr_m (p0, z0, 1))

/*
** subr_1_f64_m_untied:
**	movprfx	z0, z1
**	fsubr	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f64_m_untied, svfloat64_t,
		z0 = svsubr_n_f64_m (p0, z1, 1),
		z0 = svsubr_m (p0, z1, 1))

/*
** subr_0p5_f64_m_tied1:
**	fsubr	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f64_m_tied1, svfloat64_t,
		z0 = svsubr_n_f64_m (p0, z0, 0.5),
		z0 = svsubr_m (p0, z0, 0.5))

/*
** subr_0p5_f64_m_untied:
**	movprfx	z0, z1
**	fsubr	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f64_m_untied, svfloat64_t,
		z0 = svsubr_n_f64_m (p0, z1, 0.5),
		z0 = svsubr_m (p0, z1, 0.5))

/*
** subr_m1_f64_m_tied1:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	fsubr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f64_m_tied1, svfloat64_t,
		z0 = svsubr_n_f64_m (p0, z0, -1),
		z0 = svsubr_m (p0, z0, -1))

/*
** subr_m1_f64_m_untied:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	movprfx	z0, z1
**	fsubr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f64_m_untied, svfloat64_t,
		z0 = svsubr_n_f64_m (p0, z1, -1),
		z0 = svsubr_m (p0, z1, -1))

/*
** subr_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fsubr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_z_tied1, svfloat64_t,
		z0 = svsubr_f64_z (p0, z0, z1),
		z0 = svsubr_z (p0, z0, z1))

/*
** subr_f64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	fsub	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_z_tied2, svfloat64_t,
		z1 = svsubr_f64_z (p0, z0, z1),
		z1 = svsubr_z (p0, z0, z1))

/*
** subr_f64_z_untied:
**	movprfx	z0\.d, p0/z, z2\.d
**	fsub	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_z_untied, svfloat64_t,
		z0 = svsubr_f64_z (p0, z1, z2),
		z0 = svsubr_z (p0, z1, z2))

/*
** subr_x0_f64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	fsubr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_x0_f64_z_tied1, svfloat64_t, double,
		 z0 = svsubr_n_f64_z (p0, z0, x0),
		 z0 = svsubr_z (p0, z0, x0))

/*
** subr_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, \1
**	fsub	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (subr_x0_f64_z_untied, svfloat64_t, double,
		 z0 = svsubr_n_f64_z (p0, z1, x0),
		 z0 = svsubr_z (p0, z1, x0))

/*
** subr_d0_f64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	fsubr	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_d0_f64_z_tied1, svfloat64_t, double,
		 z1 = svsubr_n_f64_z (p0, z1, d0),
		 z1 = svsubr_z (p0, z1, d0))

/*
** subr_d0_f64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, \1
**	fsub	z1\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_ZS (subr_d0_f64_z_untied, svfloat64_t, double,
		 z1 = svsubr_n_f64_z (p0, z2, d0),
		 z1 = svsubr_z (p0, z2, d0))

/*
** subr_1_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fsubr	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f64_z_tied1, svfloat64_t,
		z0 = svsubr_n_f64_z (p0, z0, 1),
		z0 = svsubr_z (p0, z0, 1))

/*
** subr_1_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fsubr	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f64_z_untied, svfloat64_t,
		z0 = svsubr_n_f64_z (p0, z1, 1),
		z0 = svsubr_z (p0, z1, 1))

/*
** subr_0p5_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fsubr	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f64_z_tied1, svfloat64_t,
		z0 = svsubr_n_f64_z (p0, z0, 0.5),
		z0 = svsubr_z (p0, z0, 0.5))

/*
** subr_0p5_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fsubr	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f64_z_untied, svfloat64_t,
		z0 = svsubr_n_f64_z (p0, z1, 0.5),
		z0 = svsubr_z (p0, z1, 0.5))

/*
** subr_m1_f64_z_tied1:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	movprfx	z0\.d, p0/z, z0\.d
**	fsubr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f64_z_tied1, svfloat64_t,
		z0 = svsubr_n_f64_z (p0, z0, -1),
		z0 = svsubr_z (p0, z0, -1))

/*
** subr_m1_f64_z_untied:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	movprfx	z0\.d, p0/z, \1
**	fsub	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f64_z_untied, svfloat64_t,
		z0 = svsubr_n_f64_z (p0, z1, -1),
		z0 = svsubr_z (p0, z1, -1))

/*
** subr_f64_x_tied1:
**	fsub	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_x_tied1, svfloat64_t,
		z0 = svsubr_f64_x (p0, z0, z1),
		z0 = svsubr_x (p0, z0, z1))

/*
** subr_f64_x_tied2:
**	fsub	z1\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_x_tied2, svfloat64_t,
		z1 = svsubr_f64_x (p0, z0, z1),
		z1 = svsubr_x (p0, z0, z1))

/*
** subr_f64_x_untied:
**	fsub	z2\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (subr_f64_x_untied, svfloat64_t,
		z2 = svsubr_f64_x (p0, z0, z1),
		z2 = svsubr_x (p0, z0, z1))

/*
** subr_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fsub	z0\.d, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (subr_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svsubr_n_f64_x (p0, z0, x0),
		 z0 = svsubr_x (p0, z0, x0))

/*
** subr_x0_f64_x_untied:
**	mov	(z[0-9]+\.d), x0
**	fsub	z1\.d, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (subr_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svsubr_n_f64_x (p0, z0, x0),
		 z1 = svsubr_x (p0, z0, x0))

/*
** subr_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fsub	z1\.d, \1, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (subr_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svsubr_n_f64_x (p0, z1, d0),
		 z1 = svsubr_x (p0, z1, d0))

/*
** subr_d0_f64_x_untied:
**	mov	(z[0-9]+\.d), d0
**	fsub	z2\.d, \1, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (subr_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svsubr_n_f64_x (p0, z1, d0),
		 z2 = svsubr_x (p0, z1, d0))

/*
** subr_1_f64_x_tied1:
**	fsubr	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f64_x_tied1, svfloat64_t,
		z0 = svsubr_n_f64_x (p0, z0, 1),
		z0 = svsubr_x (p0, z0, 1))

/*
** subr_1_f64_x_untied:
**	movprfx	z0, z1
**	fsubr	z0\.d, p0/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f64_x_untied, svfloat64_t,
		z0 = svsubr_n_f64_x (p0, z1, 1),
		z0 = svsubr_x (p0, z1, 1))

/*
** subr_0p5_f64_x_tied1:
**	fsubr	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f64_x_tied1, svfloat64_t,
		z0 = svsubr_n_f64_x (p0, z0, 0.5),
		z0 = svsubr_x (p0, z0, 0.5))

/*
** subr_0p5_f64_x_untied:
**	movprfx	z0, z1
**	fsubr	z0\.d, p0/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f64_x_untied, svfloat64_t,
		z0 = svsubr_n_f64_x (p0, z1, 0.5),
		z0 = svsubr_x (p0, z1, 0.5))

/*
** subr_m1_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	fsub	z0\.d, \1, z0\.d
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f64_x_tied1, svfloat64_t,
		z0 = svsubr_n_f64_x (p0, z0, -1),
		z0 = svsubr_x (p0, z0, -1))

/*
** subr_m1_f64_x_untied:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	fsub	z0\.d, \1, z1\.d
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f64_x_untied, svfloat64_t,
		z0 = svsubr_n_f64_x (p0, z1, -1),
		z0 = svsubr_x (p0, z1, -1))

/*
** ptrue_subr_f64_x_tied1:
**	fsub	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_f64_x_tied1, svfloat64_t,
		z0 = svsubr_f64_x (svptrue_b64 (), z0, z1),
		z0 = svsubr_x (svptrue_b64 (), z0, z1))

/*
** ptrue_subr_f64_x_tied2:
**	fsub	z1\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_f64_x_tied2, svfloat64_t,
		z1 = svsubr_f64_x (svptrue_b64 (), z0, z1),
		z1 = svsubr_x (svptrue_b64 (), z0, z1))

/*
** ptrue_subr_f64_x_untied:
**	fsub	z2\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_f64_x_untied, svfloat64_t,
		z2 = svsubr_f64_x (svptrue_b64 (), z0, z1),
		z2 = svsubr_x (svptrue_b64 (), z0, z1))

/*
** ptrue_subr_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fsub	z0\.d, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svsubr_n_f64_x (svptrue_b64 (), z0, x0),
		 z0 = svsubr_x (svptrue_b64 (), z0, x0))

/*
** ptrue_subr_x0_f64_x_untied:
**	mov	(z[0-9]+\.d), x0
**	fsub	z1\.d, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svsubr_n_f64_x (svptrue_b64 (), z0, x0),
		 z1 = svsubr_x (svptrue_b64 (), z0, x0))

/*
** ptrue_subr_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fsub	z1\.d, \1, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svsubr_n_f64_x (svptrue_b64 (), z1, d0),
		 z1 = svsubr_x (svptrue_b64 (), z1, d0))

/*
** ptrue_subr_d0_f64_x_untied:
**	mov	(z[0-9]+\.d), d0
**	fsub	z2\.d, \1, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svsubr_n_f64_x (svptrue_b64 (), z1, d0),
		 z2 = svsubr_x (svptrue_b64 (), z1, d0))

/*
** ptrue_subr_1_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fsubr	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_1_f64_x_tied1, svfloat64_t,
		z0 = svsubr_n_f64_x (svptrue_b64 (), z0, 1),
		z0 = svsubr_x (svptrue_b64 (), z0, 1))

/*
** ptrue_subr_1_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fsubr	z0\.d, \1/m, z0\.d, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_1_f64_x_untied, svfloat64_t,
		z0 = svsubr_n_f64_x (svptrue_b64 (), z1, 1),
		z0 = svsubr_x (svptrue_b64 (), z1, 1))

/*
** ptrue_subr_0p5_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fsubr	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_0p5_f64_x_tied1, svfloat64_t,
		z0 = svsubr_n_f64_x (svptrue_b64 (), z0, 0.5),
		z0 = svsubr_x (svptrue_b64 (), z0, 0.5))

/*
** ptrue_subr_0p5_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z0, z1
**	fsubr	z0\.d, \1/m, z0\.d, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_0p5_f64_x_untied, svfloat64_t,
		z0 = svsubr_n_f64_x (svptrue_b64 (), z1, 0.5),
		z0 = svsubr_x (svptrue_b64 (), z1, 0.5))

/*
** ptrue_subr_m1_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	fsub	z0\.d, \1, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_m1_f64_x_tied1, svfloat64_t,
		z0 = svsubr_n_f64_x (svptrue_b64 (), z0, -1),
		z0 = svsubr_x (svptrue_b64 (), z0, -1))

/*
** ptrue_subr_m1_f64_x_untied:
**	fmov	(z[0-9]+\.d), #-1\.0(e\+0)?
**	fsub	z1\.d, \1, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_m1_f64_x_untied, svfloat64_t,
		z1 = svsubr_n_f64_x (svptrue_b64 (), z0, -1),
		z1 = svsubr_x (svptrue_b64 (), z0, -1))

/*
** ptrue_b32_subr_f64_x_tied1:
**	fsub	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b32_subr_f64_x_tied1, svfloat64_t,
		z0 = svsubr_f64_x (svptrue_b32 (), z0, z1),
		z0 = svsubr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_b32_subr_f64_x_tied2:
**	fsub	z1\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b32_subr_f64_x_tied2, svfloat64_t,
		z1 = svsubr_f64_x (svptrue_b32 (), z0, z1),
		z1 = svsubr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_b32_subr_f64_x_untied:
**	fsub	z2\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b32_subr_f64_x_untied, svfloat64_t,
		z2 = svsubr_f64_x (svptrue_b32 (), z0, z1),
		z2 = svsubr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_b16_subr_f64_x_tied1:
**	fsub	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_subr_f64_x_tied1, svfloat64_t,
		z0 = svsubr_f64_x (svptrue_b16 (), z0, z1),
		z0 = svsubr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_subr_f64_x_tied2:
**	fsub	z1\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_subr_f64_x_tied2, svfloat64_t,
		z1 = svsubr_f64_x (svptrue_b16 (), z0, z1),
		z1 = svsubr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_subr_f64_x_untied:
**	fsub	z2\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_subr_f64_x_untied, svfloat64_t,
		z2 = svsubr_f64_x (svptrue_b16 (), z0, z1),
		z2 = svsubr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b8_subr_f64_x_tied1:
**	fsub	z0\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_subr_f64_x_tied1, svfloat64_t,
		z0 = svsubr_f64_x (svptrue_b8 (), z0, z1),
		z0 = svsubr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_subr_f64_x_tied2:
**	fsub	z1\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_subr_f64_x_tied2, svfloat64_t,
		z1 = svsubr_f64_x (svptrue_b8 (), z0, z1),
		z1 = svsubr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_subr_f64_x_untied:
**	fsub	z2\.d, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_subr_f64_x_untied, svfloat64_t,
		z2 = svsubr_f64_x (svptrue_b8 (), z0, z1),
		z2 = svsubr_x (svptrue_b8 (), z0, z1))
