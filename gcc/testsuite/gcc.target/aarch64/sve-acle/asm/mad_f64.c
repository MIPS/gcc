/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mad_f64_m_tied1:
**	fmad	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_m_tied1, svfloat64_t,
		z0 = svmad_f64_m (p0, z0, z1, z2),
		z0 = svmad_m (p0, z0, z1, z2))

/*
** mad_f64_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.d, p0/m, \1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_m_tied2, svfloat64_t,
		z1 = svmad_f64_m (p0, z0, z1, z2),
		z1 = svmad_m (p0, z0, z1, z2))

/*
** mad_f64_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	fmad	z2\.d, p0/m, z1\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_m_tied3, svfloat64_t,
		z2 = svmad_f64_m (p0, z0, z1, z2),
		z2 = svmad_m (p0, z0, z1, z2))

/*
** mad_f64_m_untied:
**	movprfx	z0, z1
**	fmad	z0\.d, p0/m, z2\.d, z3\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_m_untied, svfloat64_t,
		z0 = svmad_f64_m (p0, z1, z2, z3),
		z0 = svmad_m (p0, z1, z2, z3))

/*
** mad_x0_f64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_m_tied1, svfloat64_t, double,
		 z0 = svmad_n_f64_m (p0, z0, z1, x0),
		 z0 = svmad_m (p0, z0, z1, x0))

/*
** mad_x0_f64_m_tied2:
**	mov	(z[0-9]+\.d), x0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_m_tied2, svfloat64_t, double,
		 z1 = svmad_n_f64_m (p0, z0, z1, x0),
		 z1 = svmad_m (p0, z0, z1, x0))

/*
** mad_x0_f64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fmad	z0\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_m_untied, svfloat64_t, double,
		 z0 = svmad_n_f64_m (p0, z1, z2, x0),
		 z0 = svmad_m (p0, z1, z2, x0))

/*
** mad_d0_f64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_m_tied1, svfloat64_t, double,
		 z0 = svmad_n_f64_m (p0, z0, z1, d0),
		 z0 = svmad_m (p0, z0, z1, d0))

/*
** mad_d0_f64_m_tied2:
**	mov	(z[0-9]+\.d), d0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_m_tied2, svfloat64_t, double,
		 z1 = svmad_n_f64_m (p0, z0, z1, d0),
		 z1 = svmad_m (p0, z0, z1, d0))

/*
** mad_d0_f64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z0
**	fmad	z1\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_m_untied, svfloat64_t, double,
		 z1 = svmad_n_f64_m (p0, z0, z2, d0),
		 z1 = svmad_m (p0, z0, z2, d0))

/*
** mad_2_f64_m_tied1:
**	fmov	(z[0-9]+\.d), #2.0(e\+0)?
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f64_m_tied1, svfloat64_t,
		z0 = svmad_n_f64_m (p0, z0, z1, 2),
		z0 = svmad_m (p0, z0, z1, 2))

/*
** mad_2_f64_m_tied2:
**	fmov	(z[0-9]+\.d), #2.0(e\+0)?
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.d, p0/m, \3\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f64_m_tied2, svfloat64_t,
		z1 = svmad_n_f64_m (p0, z0, z1, 2),
		z1 = svmad_m (p0, z0, z1, 2))

/*
** mad_2_f64_m_untied:
**	fmov	(z[0-9]+\.d), #2.0(e\+0)?
**	movprfx	z0, z1
**	fmad	z0\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f64_m_untied, svfloat64_t,
		z0 = svmad_n_f64_m (p0, z1, z2, 2),
		z0 = svmad_m (p0, z1, z2, 2))

/*
** mad_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fmad	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_z_tied1, svfloat64_t,
		z0 = svmad_f64_z (p0, z0, z1, z2),
		z0 = svmad_z (p0, z0, z1, z2))

/*
** mad_f64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	fmad	z1\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_z_tied2, svfloat64_t,
		z1 = svmad_f64_z (p0, z0, z1, z2),
		z1 = svmad_z (p0, z0, z1, z2))

/*
** mad_f64_z_tied3:
**	movprfx	z2\.d, p0/z, z2\.d
**	fmla	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_z_tied3, svfloat64_t,
		z2 = svmad_f64_z (p0, z0, z1, z2),
		z2 = svmad_z (p0, z0, z1, z2))

/*
** mad_f64_z_untied:
**	movprfx	z0\.d, p0/z, z3\.d
**	fmla	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_z_untied, svfloat64_t,
		z0 = svmad_f64_z (p0, z1, z2, z3),
		z0 = svmad_z (p0, z1, z2, z3))

/*
** mad_x0_f64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_z_tied1, svfloat64_t, double,
		 z0 = svmad_n_f64_z (p0, z0, z1, x0),
		 z0 = svmad_z (p0, z0, z1, x0))

/*
** mad_x0_f64_z_tied2:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z1\.d, p0/z, z1\.d
**	fmad	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_z_tied2, svfloat64_t, double,
		 z1 = svmad_n_f64_z (p0, z0, z1, x0),
		 z1 = svmad_z (p0, z0, z1, x0))

/*
** mad_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, \1
**	fmla	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_z_untied, svfloat64_t, double,
		 z0 = svmad_n_f64_z (p0, z1, z2, x0),
		 z0 = svmad_z (p0, z1, z2, x0))

/*
** mad_d0_f64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z0\.d, p0/z, z0\.d
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_z_tied1, svfloat64_t, double,
		 z0 = svmad_n_f64_z (p0, z0, z1, d0),
		 z0 = svmad_z (p0, z0, z1, d0))

/*
** mad_d0_f64_z_tied2:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	fmad	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_z_tied2, svfloat64_t, double,
		 z1 = svmad_n_f64_z (p0, z0, z1, d0),
		 z1 = svmad_z (p0, z0, z1, d0))

/*
** mad_d0_f64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, \1
**	fmla	z1\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_z_untied, svfloat64_t, double,
		 z1 = svmad_n_f64_z (p0, z0, z2, d0),
		 z1 = svmad_z (p0, z0, z2, d0))

/*
** mad_f64_x_tied1:
**	fmad	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_x_tied1, svfloat64_t,
		z0 = svmad_f64_x (p0, z0, z1, z2),
		z0 = svmad_x (p0, z0, z1, z2))

/*
** mad_f64_x_tied2:
**	fmad	z1\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_x_tied2, svfloat64_t,
		z1 = svmad_f64_x (p0, z0, z1, z2),
		z1 = svmad_x (p0, z0, z1, z2))

/*
** mad_f64_x_tied3:
**	fmla	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_x_tied3, svfloat64_t,
		z2 = svmad_f64_x (p0, z0, z1, z2),
		z2 = svmad_x (p0, z0, z1, z2))

/*
** mad_f64_x_untied:
**	movprfx	z3, z2
**	fmla	z3\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mad_f64_x_untied, svfloat64_t,
		z3 = svmad_f64_x (p0, z0, z1, z2),
		z3 = svmad_x (p0, z0, z1, z2))

/*
** mad_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svmad_n_f64_x (p0, z0, z1, x0),
		 z0 = svmad_x (p0, z0, z1, x0))

/*
** mad_x0_f64_x_tied2:
**	mov	(z[0-9]+\.d), x0
**	fmad	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_x_tied2, svfloat64_t, double,
		 z1 = svmad_n_f64_x (p0, z0, z1, x0),
		 z1 = svmad_x (p0, z0, z1, x0))

/*
** mad_x0_f64_x_untied:
**	mov	z2\.d, x0
**	fmla	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (mad_x0_f64_x_untied, svfloat64_t, double,
		 z2 = svmad_n_f64_x (p0, z0, z1, x0),
		 z2 = svmad_x (p0, z0, z1, x0))

/*
** mad_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_x_tied1, svfloat64_t, double,
		 z0 = svmad_n_f64_x (p0, z0, z1, d0),
		 z0 = svmad_x (p0, z0, z1, d0))

/*
** mad_d0_f64_x_tied2:
**	mov	(z[0-9]+\.d), d0
**	fmad	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_x_tied2, svfloat64_t, double,
		 z1 = svmad_n_f64_x (p0, z0, z1, d0),
		 z1 = svmad_x (p0, z0, z1, d0))

/*
** mad_d0_f64_x_untied:
**	mov	z2\.d, d0
**	fmla	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (mad_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svmad_n_f64_x (p0, z0, z1, d0),
		 z2 = svmad_x (p0, z0, z1, d0))

/*
** mad_2_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #2.0(e\+0)?
**	fmad	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f64_x_tied1, svfloat64_t,
		z0 = svmad_n_f64_x (p0, z0, z1, 2),
		z0 = svmad_x (p0, z0, z1, 2))

/*
** mad_2_f64_x_tied2:
**	fmov	(z[0-9]+\.d), #2.0(e\+0)?
**	fmad	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f64_x_tied2, svfloat64_t,
		z1 = svmad_n_f64_x (p0, z0, z1, 2),
		z1 = svmad_x (p0, z0, z1, 2))

/*
** mad_2_f64_x_untied:
**	fmov	z0\.d, #2.0(e\+0)?
**	fmla	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mad_2_f64_x_untied, svfloat64_t,
		z0 = svmad_n_f64_x (p0, z1, z2, 2),
		z0 = svmad_x (p0, z1, z2, 2))

