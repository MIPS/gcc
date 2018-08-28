/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** max_f64_m_tied1:
**	fmax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_m_tied1, svfloat64_t,
		z0 = svmax_f64_m (p0, z0, z1),
		z0 = svmax_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (max_f64_m_tied2, svfloat64_t,
		z1 = svmax_f64_m (p0, z0, z1),
		z1 = svmax_m (p0, z0, z1))

/*
** max_f64_m_untied:
**	movprfx	z0, z1
**	fmax	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_m_untied, svfloat64_t,
		z0 = svmax_f64_m (p0, z1, z2),
		z0 = svmax_m (p0, z1, z2))

/*
** max_x0_f64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_f64_m_tied1, svfloat64_t, double,
		 z0 = svmax_n_f64_m (p0, z0, x0),
		 z0 = svmax_m (p0, z0, x0))

/*
** max_x0_f64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_f64_m_untied, svfloat64_t, double,
		 z0 = svmax_n_f64_m (p0, z1, x0),
		 z0 = svmax_m (p0, z1, x0))

/*
** max_d0_f64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	fmax	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d0_f64_m_tied1, svfloat64_t, double,
		 z1 = svmax_n_f64_m (p0, z1, d0),
		 z1 = svmax_m (p0, z1, d0))

/*
** max_d0_f64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	fmax	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d0_f64_m_untied, svfloat64_t, double,
		 z1 = svmax_n_f64_m (p0, z2, d0),
		 z1 = svmax_m (p0, z2, d0))

/*
** max_1_f64_m_tied1:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f64_m_tied1, svfloat64_t,
		z0 = svmax_n_f64_m (p0, z0, 1),
		z0 = svmax_m (p0, z0, 1))

/*
** max_1_f64_m_untied:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	movprfx	z0, z1
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f64_m_untied, svfloat64_t,
		z0 = svmax_n_f64_m (p0, z1, 1),
		z0 = svmax_m (p0, z1, 1))

/*
** max_f64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	fmax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_z_tied1, svfloat64_t,
		z0 = svmax_f64_z (p0, z0, z1),
		z0 = svmax_z (p0, z0, z1))

/*
** max_f64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	fmax	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_z_tied2, svfloat64_t,
		z1 = svmax_f64_z (p0, z0, z1),
		z1 = svmax_z (p0, z0, z1))

/*
** max_f64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	fmax	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_z_untied, svfloat64_t,
		z0 = svmax_f64_z (p0, z1, z2),
		z0 = svmax_z (p0, z1, z2))

/*
** max_x0_f64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_f64_z_tied1, svfloat64_t, double,
		 z0 = svmax_n_f64_z (p0, z0, x0),
		 z0 = svmax_z (p0, z0, x0))

/*
** max_x0_f64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_f64_z_untied, svfloat64_t, double,
		 z0 = svmax_n_f64_z (p0, z1, x0),
		 z0 = svmax_z (p0, z1, x0))

/*
** max_d0_f64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	fmax	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d0_f64_z_tied1, svfloat64_t, double,
		 z1 = svmax_n_f64_z (p0, z1, d0),
		 z1 = svmax_z (p0, z1, d0))

/*
** max_d0_f64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z2\.d
**	fmax	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d0_f64_z_untied, svfloat64_t, double,
		 z1 = svmax_n_f64_z (p0, z2, d0),
		 z1 = svmax_z (p0, z2, d0))

/*
** max_1_f64_z_tied1:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	movprfx	z0\.d, p0/z, z0\.d
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f64_z_tied1, svfloat64_t,
		z0 = svmax_n_f64_z (p0, z0, 1),
		z0 = svmax_z (p0, z0, 1))

/*
** max_1_f64_z_untied:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	movprfx	z0\.d, p0/z, z1\.d
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f64_z_untied, svfloat64_t,
		z0 = svmax_n_f64_z (p0, z1, 1),
		z0 = svmax_z (p0, z1, 1))

/*
** max_f64_x_tied1:
**	fmax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_x_tied1, svfloat64_t,
		z0 = svmax_f64_x (p0, z0, z1),
		z0 = svmax_x (p0, z0, z1))

/*
** max_f64_x_tied2:
**	fmax	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_x_tied2, svfloat64_t,
		z1 = svmax_f64_x (p0, z0, z1),
		z1 = svmax_x (p0, z0, z1))

/*
** max_f64_x_untied:
**	movprfx	z2, z0
**	fmax	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_f64_x_untied, svfloat64_t,
		z2 = svmax_f64_x (p0, z0, z1),
		z2 = svmax_x (p0, z0, z1))

/*
** max_x0_f64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_f64_x_tied1, svfloat64_t, double,
		 z0 = svmax_n_f64_x (p0, z0, x0),
		 z0 = svmax_x (p0, z0, x0))

/*
** max_x0_f64_x_untied:
**	mov	z1\.d, x0
**	fmax	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (max_x0_f64_x_untied, svfloat64_t, double,
		 z1 = svmax_n_f64_x (p0, z0, x0),
		 z1 = svmax_x (p0, z0, x0))

/*
** max_d0_f64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	fmax	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d0_f64_x_tied1, svfloat64_t, double,
		 z1 = svmax_n_f64_x (p0, z1, d0),
		 z1 = svmax_x (p0, z1, d0))

/*
** max_d0_f64_x_untied:
**	mov	z2\.d, d0
**	fmax	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (max_d0_f64_x_untied, svfloat64_t, double,
		 z2 = svmax_n_f64_x (p0, z1, d0),
		 z2 = svmax_x (p0, z1, d0))

/*
** max_1_f64_x_tied1:
**	fmov	(z[0-9]+\.d), #1.0(e\+0)?
**	fmax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f64_x_tied1, svfloat64_t,
		z0 = svmax_n_f64_x (p0, z0, 1),
		z0 = svmax_x (p0, z0, 1))

/*
** max_1_f64_x_untied:
**	fmov	z0\.d, #1.0(e\+0)?
**	fmax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_1_f64_x_untied, svfloat64_t,
		z0 = svmax_n_f64_x (p0, z1, 1),
		z0 = svmax_x (p0, z1, 1))

/*
** ptrue_max_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fmax	z0\.d, \1/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_max_f64_x_tied1, svfloat64_t,
		z0 = svmax_f64_x (svptrue_b64 (), z0, z1),
		z0 = svmax_x (svptrue_b64 (), z0, z1))

/*
** ptrue_max_f64_x_tied2:
**	ptrue	(p[0-7])\.d[^\n]*
**	fmax	z1\.d, \1/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_max_f64_x_tied2, svfloat64_t,
		z1 = svmax_f64_x (svptrue_b64 (), z0, z1),
		z1 = svmax_x (svptrue_b64 (), z0, z1))

/*
** ptrue_max_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	movprfx	z2, z0
**	fmax	z2\.d, \1/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (ptrue_max_f64_x_untied, svfloat64_t,
		z2 = svmax_f64_x (svptrue_b64 (), z0, z1),
		z2 = svmax_x (svptrue_b64 (), z0, z1))
