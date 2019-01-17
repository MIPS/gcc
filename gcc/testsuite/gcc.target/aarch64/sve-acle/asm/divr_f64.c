/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** divr_f64_m_tied1:
**	fdivr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_m_tied1, svfloat64_t,
		z0 = svdivr_f64_m (p0, z0, z1),
		z0 = svdivr_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (divr_f64_m_tied2, svfloat64_t,
		z1 = svdivr_f64_m (p0, z0, z1),
		z1 = svdivr_m (p0, z0, z1))

/*
** divr_f64_m_untied:
**	movprfx	z0, z1
**	fdivr	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_m_untied, svfloat64_t,
		z0 = svdivr_f64_m (p0, z1, z2),
		z0 = svdivr_m (p0, z1, z2))

/*
** divr_x0_f64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_x0_f64_m_tied1, svfloat64_t, double,
		 z0 = svdivr_n_f64_m (p0, z0, x0),
		 z0 = svdivr_m (p0, z0, x0))

/*
** divr_x0_f64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_x0_f64_m_untied, svfloat64_t, double,
		 z0 = svdivr_n_f64_m (p0, z1, x0),
		 z0 = svdivr_m (p0, z1, x0))

/*
** divr_d0_f64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	fdivr	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_d0_f64_m_tied1, svfloat64_t, double,
		 z1 = svdivr_n_f64_m (p0, z1, d0),
		 z1 = svdivr_m (p0, z1, d0))

/*
** divr_d0_f64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	fdivr	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_d0_f64_m_untied, svfloat64_t, double,
		 z1 = svdivr_n_f64_m (p0, z2, d0),
		 z1 = svdivr_m (p0, z2, d0))

/*
** divr_1_f64_m_tied1:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f64_m_tied1, svfloat64_t,
		z0 = svdivr_n_f64_m (p0, z0, 1),
		z0 = svdivr_m (p0, z0, 1))

/*
** divr_1_f64_m_untied:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	movprfx	z0, z1
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f64_m_untied, svfloat64_t,
		z0 = svdivr_n_f64_m (p0, z1, 1),
		z0 = svdivr_m (p0, z1, 1))

/*
** divr_0p5_f64_m_tied1:
**	fmov	(z[0-9]+\.d), #5.0(e\-1)?
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f64_m_tied1, svfloat64_t,
		z0 = svdivr_n_f64_m (p0, z0, 0.5),
		z0 = svdivr_m (p0, z0, 0.5))

/*
** divr_0p5_f64_m_untied:
**	fmov	(z[0-9]+\.d), #5.0(e\-1)?
**	movprfx	z0, z1
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f64_m_untied, svfloat64_t,
		z0 = svdivr_n_f64_m (p0, z1, 0.5),
		z0 = svdivr_m (p0, z1, 0.5))


/*
** divr_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fdivr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_z_tied1, svfloat64_t,
		z0 = svdivr_f64_z (p0, z0, z1),
		z0 = svdivr_z (p0, z0, z1))

/*
** divr_f64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	fdiv	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_z_tied2, svfloat64_t,
		z1 = svdivr_f64_z (p0, z0, z1),
		z1 = svdivr_z (p0, z0, z1))

/*
** divr_f64_z_untied:
**	movprfx	z0\.d, p0/z, z2\.d
**	fdiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_z_untied, svfloat64_t,
		z0 = svdivr_f64_z (p0, z1, z2),
		z0 = svdivr_z (p0, z1, z2))

/*
** divr_x0_f64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_x0_f64_z_tied1, svfloat64_t, double,
		 z0 = svdivr_n_f64_z (p0, z0, x0),
		 z0 = svdivr_z (p0, z0, x0))

/*
** divr_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, \1
**	fdiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (divr_x0_f64_z_untied, svfloat64_t, double,
		 z0 = svdivr_n_f64_z (p0, z1, x0),
		 z0 = svdivr_z (p0, z1, x0))

/*
** divr_d0_f64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	fdivr	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_d0_f64_z_tied1, svfloat64_t, double,
		 z1 = svdivr_n_f64_z (p0, z1, d0),
		 z1 = svdivr_z (p0, z1, d0))

/*
** divr_d0_f64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, \1
**	fdiv	z1\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_ZS (divr_d0_f64_z_untied, svfloat64_t, double,
		 z1 = svdivr_n_f64_z (p0, z2, d0),
		 z1 = svdivr_z (p0, z2, d0))

/*
** divr_1_f64_z_tied1:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	movprfx	z0\.d, p0/z, z0\.d
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f64_z_tied1, svfloat64_t,
		z0 = svdivr_n_f64_z (p0, z0, 1),
		z0 = svdivr_z (p0, z0, 1))

/*
** divr_0p5_f64_z_tied1:
**	fmov	(z[0-9]+\.d), #5.0(e\-1)?
**	movprfx	z0\.d, p0/z, z0\.d
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f64_z_tied1, svfloat64_t,
		z0 = svdivr_n_f64_z (p0, z0, 0.5),
		z0 = svdivr_z (p0, z0, 0.5))

/*
** divr_0p5_f64_z_untied:
**	fmov	(z[0-9]+\.d), #5.0(e\-1)?
**	movprfx	z0\.d, p0/z, \1
**	fdiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f64_z_untied, svfloat64_t,
		z0 = svdivr_n_f64_z (p0, z1, 0.5),
		z0 = svdivr_z (p0, z1, 0.5))

/*
** divr_f64_x_tied1:
**	fdivr	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_x_tied1, svfloat64_t,
		z0 = svdivr_f64_x (p0, z0, z1),
		z0 = svdivr_x (p0, z0, z1))

/*
** divr_f64_x_tied2:
**	fdiv	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_x_tied2, svfloat64_t,
		z1 = svdivr_f64_x (p0, z0, z1),
		z1 = svdivr_x (p0, z0, z1))

/*
** divr_f64_x_untied:
**	movprfx	z2, z1
**	fdiv	z2\.d, p0/m, z2\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (divr_f64_x_untied, svfloat64_t,
		z2 = svdivr_f64_x (p0, z0, z1),
		z2 = svdivr_x (p0, z0, z1))

/*
** divr_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svdivr_n_f64_x (p0, z0, x0),
		 z0 = svdivr_x (p0, z0, x0))

/*
** divr_x0_f64_x_untied:
**	mov	z1\.d, x0
**	fdiv	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (divr_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svdivr_n_f64_x (p0, z0, x0),
		 z1 = svdivr_x (p0, z0, x0))

/*
** divr_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fdivr	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svdivr_n_f64_x (p0, z1, d0),
		 z1 = svdivr_x (p0, z1, d0))

/*
** divr_d0_f64_x_untied:
**	mov	z2\.d, d0
**	fdiv	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (divr_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svdivr_n_f64_x (p0, z1, d0),
		 z2 = svdivr_x (p0, z1, d0))

/*
** divr_1_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #1\.0(e\+0)?
**	fdivr	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f64_x_tied1, svfloat64_t,
		z0 = svdivr_n_f64_x (p0, z0, 1),
		z0 = svdivr_x (p0, z0, 1))

/*
** divr_1_f64_x_untied:
**	fmov	z0\.d, #1\.0(e\+0)?
**	fdiv	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (divr_1_f64_x_untied, svfloat64_t,
		z0 = svdivr_n_f64_x (p0, z1, 1),
		z0 = svdivr_x (p0, z1, 1))

/*
** ptrue_divr_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fdivr	z0\.d, \1/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f64_x_tied1, svfloat64_t,
		z0 = svdivr_f64_x (svptrue_b64 (), z0, z1),
		z0 = svdivr_x (svptrue_b64 (), z0, z1))

/*
** ptrue_divr_f64_x_tied2:
**	ptrue	(p[0-7])\.d[^\n]*
**	fdiv	z1\.d, \1/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f64_x_tied2, svfloat64_t,
		z1 = svdivr_f64_x (svptrue_b64 (), z0, z1),
		z1 = svdivr_x (svptrue_b64 (), z0, z1))

/*
** ptrue_divr_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z2, z1
**	fdiv	z2\.d, \1/m, z2\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f64_x_untied, svfloat64_t,
		z2 = svdivr_f64_x (svptrue_b64 (), z0, z1),
		z2 = svdivr_x (svptrue_b64 (), z0, z1))

/*
** ptrue_divr_x0_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), x0
**	fdivr	z0\.d, \1/m, z0\.d, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svdivr_n_f64_x (svptrue_b64 (), z0, x0),
		 z0 = svdivr_x (svptrue_b64 (), z0, x0))

/*
** ptrue_divr_x0_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	z1\.d, x0
**	fdiv	z1\.d, \1/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svdivr_n_f64_x (svptrue_b64 (), z0, x0),
		 z1 = svdivr_x (svptrue_b64 (), z0, x0))

/*
** ptrue_divr_d0_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	(z[0-9]+\.d), d0
**	fdivr	z1\.d, \1/m, z1\.d, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svdivr_n_f64_x (svptrue_b64 (), z1, d0),
		 z1 = svdivr_x (svptrue_b64 (), z1, d0))

/*
** ptrue_divr_d0_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	mov	z2\.d, d0
**	fdiv	z2\.d, \1/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svdivr_n_f64_x (svptrue_b64 (), z1, d0),
		 z2 = svdivr_x (svptrue_b64 (), z1, d0))

/*
** ptrue_divr_1_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	fmov	z0\.d, #1.0(e\+0)?
**	fdiv	z0\.d, \1/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_1_f64_x_untied, svfloat64_t,
		z0 = svdivr_n_f64_x (svptrue_b64 (), z1, 1),
		z0 = svdivr_x (svptrue_b64 (), z1, 1))

/*
** ptrue_b8_divr_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fdivr	z0\.d, \1/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f64_x_tied1, svfloat64_t,
		z0 = svdivr_f64_x (svptrue_b8 (), z0, z1),
		z0 = svdivr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_divr_f64_x_tied2:
**	ptrue	(p[0-7])\.d[^\n]*
**	fdiv	z1\.d, \1/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f64_x_tied2, svfloat64_t,
		z1 = svdivr_f64_x (svptrue_b8 (), z0, z1),
		z1 = svdivr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_divr_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z2, z1
**	fdiv	z2\.d, \1/m, z2\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f64_x_untied, svfloat64_t,
		z2 = svdivr_f64_x (svptrue_b8 (), z0, z1),
		z2 = svdivr_x (svptrue_b8 (), z0, z1))
