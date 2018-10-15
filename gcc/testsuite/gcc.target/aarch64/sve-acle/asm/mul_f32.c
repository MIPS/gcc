/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mul_f32_m_tied1:
**	fmul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_m_tied1, svfloat32_t,
		z0 = svmul_f32_m (p0, z0, z1),
		z0 = svmul_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mul_f32_m_tied2, svfloat32_t,
		z1 = svmul_f32_m (p0, z0, z1),
		z1 = svmul_m (p0, z0, z1))

/*
** mul_f32_m_untied:
**	movprfx	z0, z1
**	fmul	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_m_untied, svfloat32_t,
		z0 = svmul_f32_m (p0, z1, z2),
		z0 = svmul_m (p0, z1, z2))

/*
** mul_w0_f32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_f32_m_tied1, svfloat32_t, float,
		 z0 = svmul_n_f32_m (p0, z0, x0),
		 z0 = svmul_m (p0, z0, x0))

/*
** mul_w0_f32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_f32_m_untied, svfloat32_t, float,
		 z0 = svmul_n_f32_m (p0, z1, x0),
		 z0 = svmul_m (p0, z1, x0))

/*
** mul_s0_f32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	fmul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_f32_m_tied1, svfloat32_t, float,
		 z1 = svmul_n_f32_m (p0, z1, d0),
		 z1 = svmul_m (p0, z1, d0))

/*
** mul_s0_f32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	fmul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_f32_m_untied, svfloat32_t, float,
		 z1 = svmul_n_f32_m (p0, z2, d0),
		 z1 = svmul_m (p0, z2, d0))

/*
** mul_1_f32_m_tied1:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f32_m_tied1, svfloat32_t,
		z0 = svmul_n_f32_m (p0, z0, 1),
		z0 = svmul_m (p0, z0, 1))

/*
** mul_1_f32_m_untied:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	movprfx	z0, z1
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f32_m_untied, svfloat32_t,
		z0 = svmul_n_f32_m (p0, z1, 1),
		z0 = svmul_m (p0, z1, 1))

/*
** mul_0p5_f32_m_tied1:
**	fmul	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f32_m_tied1, svfloat32_t,
		z0 = svmul_n_f32_m (p0, z0, 0.5),
		z0 = svmul_m (p0, z0, 0.5))

/*
** mul_0p5_f32_m_untied:
**	movprfx	z0, z1
**	fmul	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f32_m_untied, svfloat32_t,
		z0 = svmul_n_f32_m (p0, z1, 0.5),
		z0 = svmul_m (p0, z1, 0.5))

/*
** mul_2_f32_m_tied1:
**	fmul	z0\.s, p0/m, z0\.s, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f32_m_tied1, svfloat32_t,
		z0 = svmul_n_f32_m (p0, z0, 2.0),
		z0 = svmul_m (p0, z0, 2.0))

/*
** mul_2_f32_m_untied:
**	movprfx	z0, z1
**	fmul	z0\.s, p0/m, z0\.s, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f32_m_untied, svfloat32_t,
		z0 = svmul_n_f32_m (p0, z1, 2.0),
		z0 = svmul_m (p0, z1, 2.0))

/*
** mul_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fmul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_z_tied1, svfloat32_t,
		z0 = svmul_f32_z (p0, z0, z1),
		z0 = svmul_z (p0, z0, z1))

/*
** mul_f32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	fmul	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_z_tied2, svfloat32_t,
		z1 = svmul_f32_z (p0, z0, z1),
		z1 = svmul_z (p0, z0, z1))

/*
** mul_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fmul	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_z_untied, svfloat32_t,
		z0 = svmul_f32_z (p0, z1, z2),
		z0 = svmul_z (p0, z1, z2))

/*
** mul_w0_f32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_f32_z_tied1, svfloat32_t, float,
		 z0 = svmul_n_f32_z (p0, z0, x0),
		 z0 = svmul_z (p0, z0, x0))

/*
** mul_w0_f32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, \1
**	fmul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_f32_z_untied, svfloat32_t, float,
		 z0 = svmul_n_f32_z (p0, z1, x0),
		 z0 = svmul_z (p0, z1, x0))

/*
** mul_s0_f32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	(z1\.s, p0/z, z1\.s|z1\.s, p0/z, \1)
**	fmul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_f32_z_tied1, svfloat32_t, float,
		 z1 = svmul_n_f32_z (p0, z1, d0),
		 z1 = svmul_z (p0, z1, d0))

/*
** mul_s0_f32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, \1
**	fmul	z1\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_f32_z_untied, svfloat32_t, float,
		 z1 = svmul_n_f32_z (p0, z2, d0),
		 z1 = svmul_z (p0, z2, d0))

/*
** mul_1_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f32_z_tied1, svfloat32_t,
		z0 = svmul_n_f32_z (p0, z0, 1),
		z0 = svmul_z (p0, z0, 1))

/*
** mul_0p5_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fmul	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f32_z_tied1, svfloat32_t,
		z0 = svmul_n_f32_z (p0, z0, 0.5),
		z0 = svmul_z (p0, z0, 0.5))

/*
** mul_0p5_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fmul	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f32_z_untied, svfloat32_t,
		z0 = svmul_n_f32_z (p0, z1, 0.5),
		z0 = svmul_z (p0, z1, 0.5))

/*
** mul_2_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fmul	z0\.s, p0/m, z0\.s, #2.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f32_z_untied, svfloat32_t,
		z0 = svmul_n_f32_z (p0, z1, 2),
		z0 = svmul_z (p0, z1, 2))

/*
** mul_f32_x_tied1:
**	fmul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_x_tied1, svfloat32_t,
		z0 = svmul_f32_x (p0, z0, z1),
		z0 = svmul_x (p0, z0, z1))

/*
** mul_f32_x_tied2:
**	fmul	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_x_tied2, svfloat32_t,
		z1 = svmul_f32_x (p0, z0, z1),
		z1 = svmul_x (p0, z0, z1))

/*
** mul_f32_x_untied:
**	movprfx	z2, z0
**	fmul	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_f32_x_untied, svfloat32_t,
		z2 = svmul_f32_x (p0, z0, z1),
		z2 = svmul_x (p0, z0, z1))

/*
** mul_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svmul_n_f32_x (p0, z0, x0),
		 z0 = svmul_x (p0, z0, x0))

/*
** mul_w0_f32_x_untied:
**	mov	z1\.s, w0
**	fmul	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svmul_n_f32_x (p0, z0, x0),
		 z1 = svmul_x (p0, z0, x0))

/*
** mul_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fmul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svmul_n_f32_x (p0, z1, d0),
		 z1 = svmul_x (p0, z1, d0))

/*
** mul_s0_f32_x_untied:
**	mov	z2\.s, s0
**	fmul	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svmul_n_f32_x (p0, z1, d0),
		 z2 = svmul_x (p0, z1, d0))

/*
** mul_1_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #1\.0(e\+0)?
**	fmul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_1_f32_x_tied1, svfloat32_t,
		z0 = svmul_n_f32_x (p0, z0, 1),
		z0 = svmul_x (p0, z0, 1))

/*
** mul_1_f32_x_untied:
**	fmov	z0\.s, #1\.0(e\+0)?
**	fmul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_1_f32_x_untied, svfloat32_t,
		z0 = svmul_n_f32_x (p0, z1, 1),
		z0 = svmul_x (p0, z1, 1))

/*
** mul_0p5_f32_x_tied1:
**	fmul	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f32_x_tied1, svfloat32_t,
		z0 = svmul_n_f32_x (p0, z0, 0.5),
		z0 = svmul_x (p0, z0, 0.5))

/*
** mul_0p5_f32_x_untied:
**	movprfx	z0, z1
**	fmul	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (mul_0p5_f32_x_untied, svfloat32_t,
		z0 = svmul_n_f32_x (p0, z1, 0.5),
		z0 = svmul_x (p0, z1, 0.5))

/*
** mul_2_f32_x_tied1:
**	fmul	z0\.s, p0/m, z0\.s, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f32_x_tied1, svfloat32_t,
		z0 = svmul_n_f32_x (p0, z0, 2),
		z0 = svmul_x (p0, z0, 2))

/*
** mul_2_f32_x_untied:
**	movprfx	z0, z1
**	fmul	z0\.s, p0/m, z0\.s, #2\.0
**	ret
*/
TEST_UNIFORM_Z (mul_2_f32_x_untied, svfloat32_t,
		z0 = svmul_n_f32_x (p0, z1, 2),
		z0 = svmul_x (p0, z1, 2))

/*
** ptrue_mul_f32_x_tied1:
**	fmul	(z0\.s, z1\.s, z0\.s|z0\.s, z0\.s, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_f32_x_tied1, svfloat32_t,
		z0 = svmul_f32_x (svptrue_b32 (), z0, z1),
		z0 = svmul_x (svptrue_b32 (), z0, z1))

/*
** ptrue_mul_f32_x_tied2:
**	fmul	(z1\.s, z1\.s, z0\.s|z1\.s, z0\.s, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_f32_x_tied2, svfloat32_t,
		z1 = svmul_f32_x (svptrue_b32 (), z0, z1),
		z1 = svmul_x (svptrue_b32 (), z0, z1))

/*
** ptrue_mul_f32_x_untied:
**	fmul	(z2\.s, z1\.s, z0\.s|z2\.s, z0\.s, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_f32_x_untied, svfloat32_t,
		z2 = svmul_f32_x (svptrue_b32 (), z0, z1),
		z2 = svmul_x (svptrue_b32 (), z0, z1))

/*
** ptrue_mul_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fmul	(z0\.s, \1, z0\.s|z0\.s, z0\.s, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svmul_n_f32_x (svptrue_b32 (), z0, x0),
		 z0 = svmul_x (svptrue_b32 (), z0, x0))

/*
** ptrue_mul_w0_f32_x_untied:
**	mov	(z[0-9]+\.s), w0
**	fmul	(z1\.s, \1, z0\.s|z1\.s, z0\.s, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svmul_n_f32_x (svptrue_b32 (), z0, x0),
		 z1 = svmul_x (svptrue_b32 (), z0, x0))

/*
** ptrue_mul_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fmul	(z1\.s, \1, z1\.s|z1\.s, z1\.s, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svmul_n_f32_x (svptrue_b32 (), z1, d0),
		 z1 = svmul_x (svptrue_b32 (), z1, d0))

/*
** ptrue_mul_s0_f32_x_untied:
**	mov	(z[0-9]+\.s), s0
**	fmul	(z2\.s, \1, z1\.s|z2\.s, z1\.s, \1)
**	ret
*/
TEST_UNIFORM_ZS (ptrue_mul_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svmul_n_f32_x (svptrue_b32 (), z1, d0),
		 z2 = svmul_x (svptrue_b32 (), z1, d0))

/*
** ptrue_mul_1_f32_x_untied:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	fmul	(z0\.s, \1, z1\.s|z0\.s, z1\.s, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_1_f32_x_untied, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z1, 1),
		z0 = svmul_x (svptrue_b32 (), z1, 1))

/*
** ptrue_mul_0p5_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmul	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_0p5_f32_x_tied1, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z0, 0.5),
		z0 = svmul_x (svptrue_b32 (), z0, 0.5))

/*
** ptrue_mul_0p5_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z0, z1
**	fmul	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_0p5_f32_x_untied, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z1, 0.5),
		z0 = svmul_x (svptrue_b32 (), z1, 0.5))

/*
** ptrue_mul_m1_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-1.0(e\+0)?
**	fmul	(z0\.s, \1, z0\.s|z0\.s, z0\.s, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m1_f32_x_tied1, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z0, -1),
		z0 = svmul_x (svptrue_b32 (), z0, -1))

/*
** ptrue_mul_m1_f32_x_untied:
**	fmov	(z[0-9]+\.s), #-1.0(e\+0)?
**	fmul	(z0\.s, \1, z1\.s|z0\.s, z1\.s, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m1_f32_x_untied, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z1, -1),
		z0 = svmul_x (svptrue_b32 (), z1, -1))

/*
** ptrue_mul_m0p5_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-5.0(e\-1)?
**	fmul	(z0\.s, \1, z0\.s|z0\.s, z0\.s, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m0p5_f32_x_tied1, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z0, -0.5),
		z0 = svmul_x (svptrue_b32 (), z0, -0.5))

/*
** ptrue_mul_m0p5_f32_x_untied:
**	fmov	(z[0-9]+\.s), #-5.0(e\-1)?
**	fmul	(z0\.s, \1, z1\.s|z0\.s, z1\.s, \1)
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_m0p5_f32_x_untied, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z1, -0.5),
		z0 = svmul_x (svptrue_b32 (), z1, -0.5))

/*
** ptrue_mul_2_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmul	z0\.s, \1/m, z0\.s, #2.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_2_f32_x_tied1, svfloat32_t,
		z0 = svmul_n_f32_x (svptrue_b32 (), z0, 2),
		z0 = svmul_x (svptrue_b32 (), z0, 2))

/*
** ptrue_mul_2_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z1, z0
**	fmul	z1\.s, \1/m, z1\.s, #2.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_mul_2_f32_x_untied, svfloat32_t,
		z1 = svmul_n_f32_x (svptrue_b32 (), z0, 2),
		z1 = svmul_x (svptrue_b32 (), z0, 2))

/*
** ptrue_b8_mul_f32_x_tied1:
**	fmul	(z0\.s, z1\.s, z0\.s|z0\.s, z0\.s, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_mul_f32_x_tied1, svfloat32_t,
		z0 = svmul_f32_x (svptrue_b8 (), z0, z1),
		z0 = svmul_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_mul_f32_x_tied2:
**	fmul	(z1\.s, z1\.s, z0\.s|z1\.s, z0\.s, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_mul_f32_x_tied2, svfloat32_t,
		z1 = svmul_f32_x (svptrue_b8 (), z0, z1),
		z1 = svmul_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_mul_f32_x_untied:
**	fmul	(z2\.s, z1\.s, z0\.s|z2\.s, z0\.s, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_mul_f32_x_untied, svfloat32_t,
		z2 = svmul_f32_x (svptrue_b8 (), z0, z1),
		z2 = svmul_x (svptrue_b8 (), z0, z1))
