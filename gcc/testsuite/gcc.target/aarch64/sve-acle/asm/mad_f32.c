/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mad_f32_m_tied1:
**	fmad	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_m_tied1, svfloat32_t,
		z0 = svmad_f32_m (p0, z0, z1, z2),
		z0 = svmad_m (p0, z0, z1, z2))

/*
** mad_f32_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.s, p0/m, \1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_m_tied2, svfloat32_t,
		z1 = svmad_f32_m (p0, z0, z1, z2),
		z1 = svmad_m (p0, z0, z1, z2))

/*
** mad_f32_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	fmad	z2\.s, p0/m, z1\.s, \1\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_m_tied3, svfloat32_t,
		z2 = svmad_f32_m (p0, z0, z1, z2),
		z2 = svmad_m (p0, z0, z1, z2))

/*
** mad_f32_m_untied:
**	movprfx	z0, z1
**	fmad	z0\.s, p0/m, z2\.s, z3\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_m_untied, svfloat32_t,
		z0 = svmad_f32_m (p0, z1, z2, z3),
		z0 = svmad_m (p0, z1, z2, z3))

/*
** mad_w0_f32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_m_tied1, svfloat32_t, float,
		 z0 = svmad_n_f32_m (p0, z0, z1, x0),
		 z0 = svmad_m (p0, z0, z1, x0))

/*
** mad_w0_f32_m_tied2:
**	mov	(z[0-9]+\.s), w0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.s, p0/m, \2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_m_tied2, svfloat32_t, float,
		 z1 = svmad_n_f32_m (p0, z0, z1, x0),
		 z1 = svmad_m (p0, z0, z1, x0))

/*
** mad_w0_f32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	fmad	z0\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_m_untied, svfloat32_t, float,
		 z0 = svmad_n_f32_m (p0, z1, z2, x0),
		 z0 = svmad_m (p0, z1, z2, x0))

/*
** mad_s0_f32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_m_tied1, svfloat32_t, float,
		 z0 = svmad_n_f32_m (p0, z0, z1, d0),
		 z0 = svmad_m (p0, z0, z1, d0))

/*
** mad_s0_f32_m_tied2:
**	mov	(z[0-9]+\.s), s0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.s, p0/m, \2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_m_tied2, svfloat32_t, float,
		 z1 = svmad_n_f32_m (p0, z0, z1, d0),
		 z1 = svmad_m (p0, z0, z1, d0))

/*
** mad_s0_f32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z0
**	fmad	z1\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_m_untied, svfloat32_t, float,
		 z1 = svmad_n_f32_m (p0, z0, z2, d0),
		 z1 = svmad_m (p0, z0, z2, d0))

/*
** mad_2_f32_m_tied1:
**	fmov	(z[0-9]+\.s), #2.0(e\+0)?
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f32_m_tied1, svfloat32_t,
		z0 = svmad_n_f32_m (p0, z0, z1, 2),
		z0 = svmad_m (p0, z0, z1, 2))

/*
** mad_2_f32_m_tied2:
**	fmov	(z[0-9]+\.s), #2.0(e\+0)?
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmad	z1\.s, p0/m, \3\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f32_m_tied2, svfloat32_t,
		z1 = svmad_n_f32_m (p0, z0, z1, 2),
		z1 = svmad_m (p0, z0, z1, 2))

/*
** mad_2_f32_m_untied:
**	fmov	(z[0-9]+\.s), #2.0(e\+0)?
**	movprfx	z0, z1
**	fmad	z0\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f32_m_untied, svfloat32_t,
		z0 = svmad_n_f32_m (p0, z1, z2, 2),
		z0 = svmad_m (p0, z1, z2, 2))

/*
** mad_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fmad	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_z_tied1, svfloat32_t,
		z0 = svmad_f32_z (p0, z0, z1, z2),
		z0 = svmad_z (p0, z0, z1, z2))

/*
** mad_f32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	fmad	z1\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_z_tied2, svfloat32_t,
		z1 = svmad_f32_z (p0, z0, z1, z2),
		z1 = svmad_z (p0, z0, z1, z2))

/*
** mad_f32_z_tied3:
**	movprfx	z2\.s, p0/z, z2\.s
**	fmla	z2\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_z_tied3, svfloat32_t,
		z2 = svmad_f32_z (p0, z0, z1, z2),
		z2 = svmad_z (p0, z0, z1, z2))

/*
** mad_f32_z_untied:
**	movprfx	z0\.s, p0/z, z3\.s
**	fmla	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_z_untied, svfloat32_t,
		z0 = svmad_f32_z (p0, z1, z2, z3),
		z0 = svmad_z (p0, z1, z2, z3))

/*
** mad_w0_f32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_z_tied1, svfloat32_t, float,
		 z0 = svmad_n_f32_z (p0, z0, z1, x0),
		 z0 = svmad_z (p0, z0, z1, x0))

/*
** mad_w0_f32_z_tied2:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z1\.s, p0/z, z1\.s
**	fmad	z1\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_z_tied2, svfloat32_t, float,
		 z1 = svmad_n_f32_z (p0, z0, z1, x0),
		 z1 = svmad_z (p0, z0, z1, x0))

/*
** mad_w0_f32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, \1
**	fmla	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_z_untied, svfloat32_t, float,
		 z0 = svmad_n_f32_z (p0, z1, z2, x0),
		 z0 = svmad_z (p0, z1, z2, x0))

/*
** mad_s0_f32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z0\.s, p0/z, z0\.s
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_z_tied1, svfloat32_t, float,
		 z0 = svmad_n_f32_z (p0, z0, z1, d0),
		 z0 = svmad_z (p0, z0, z1, d0))

/*
** mad_s0_f32_z_tied2:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	fmad	z1\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_z_tied2, svfloat32_t, float,
		 z1 = svmad_n_f32_z (p0, z0, z1, d0),
		 z1 = svmad_z (p0, z0, z1, d0))

/*
** mad_s0_f32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, \1
**	fmla	z1\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_z_untied, svfloat32_t, float,
		 z1 = svmad_n_f32_z (p0, z0, z2, d0),
		 z1 = svmad_z (p0, z0, z2, d0))

/*
** mad_f32_x_tied1:
**	fmad	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_x_tied1, svfloat32_t,
		z0 = svmad_f32_x (p0, z0, z1, z2),
		z0 = svmad_x (p0, z0, z1, z2))

/*
** mad_f32_x_tied2:
**	fmad	z1\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_x_tied2, svfloat32_t,
		z1 = svmad_f32_x (p0, z0, z1, z2),
		z1 = svmad_x (p0, z0, z1, z2))

/*
** mad_f32_x_tied3:
**	fmla	z2\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_x_tied3, svfloat32_t,
		z2 = svmad_f32_x (p0, z0, z1, z2),
		z2 = svmad_x (p0, z0, z1, z2))

/*
** mad_f32_x_untied:
**	movprfx	z3, z2
**	fmla	z3\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mad_f32_x_untied, svfloat32_t,
		z3 = svmad_f32_x (p0, z0, z1, z2),
		z3 = svmad_x (p0, z0, z1, z2))

/*
** mad_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svmad_n_f32_x (p0, z0, z1, x0),
		 z0 = svmad_x (p0, z0, z1, x0))

/*
** mad_w0_f32_x_tied2:
**	mov	(z[0-9]+\.s), w0
**	fmad	z1\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_x_tied2, svfloat32_t, float,
		 z1 = svmad_n_f32_x (p0, z0, z1, x0),
		 z1 = svmad_x (p0, z0, z1, x0))

/*
** mad_w0_f32_x_untied:
**	mov	z2\.s, w0
**	fmla	z2\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_f32_x_untied, svfloat32_t, float,
		 z2 = svmad_n_f32_x (p0, z0, z1, x0),
		 z2 = svmad_x (p0, z0, z1, x0))

/*
** mad_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_x_tied1, svfloat32_t, float,
		 z0 = svmad_n_f32_x (p0, z0, z1, d0),
		 z0 = svmad_x (p0, z0, z1, d0))

/*
** mad_s0_f32_x_tied2:
**	mov	(z[0-9]+\.s), s0
**	fmad	z1\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_x_tied2, svfloat32_t, float,
		 z1 = svmad_n_f32_x (p0, z0, z1, d0),
		 z1 = svmad_x (p0, z0, z1, d0))

/*
** mad_s0_f32_x_untied:
**	mov	z2\.s, s0
**	fmla	z2\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (mad_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svmad_n_f32_x (p0, z0, z1, d0),
		 z2 = svmad_x (p0, z0, z1, d0))

/*
** mad_2_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #2.0(e\+0)?
**	fmad	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f32_x_tied1, svfloat32_t,
		z0 = svmad_n_f32_x (p0, z0, z1, 2),
		z0 = svmad_x (p0, z0, z1, 2))

/*
** mad_2_f32_x_tied2:
**	fmov	(z[0-9]+\.s), #2.0(e\+0)?
**	fmad	z1\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_f32_x_tied2, svfloat32_t,
		z1 = svmad_n_f32_x (p0, z0, z1, 2),
		z1 = svmad_x (p0, z0, z1, 2))

/*
** mad_2_f32_x_untied:
**	fmov	z0\.s, #2.0(e\+0)?
**	fmla	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mad_2_f32_x_untied, svfloat32_t,
		z0 = svmad_n_f32_x (p0, z1, z2, 2),
		z0 = svmad_x (p0, z1, z2, 2))

