/* { dg-do compile } */
/* { dg-additional-options "-fno-trapping-math" } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** sub_f32_m_tied1:
**	fsub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_m_tied1, svfloat32_t,
		z0 = svsub_f32_m (p0, z0, z1),
		z0 = svsub_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (sub_f32_m_tied2, svfloat32_t,
		z1 = svsub_f32_m (p0, z0, z1),
		z1 = svsub_m (p0, z0, z1))

/*
** sub_f32_m_untied:
**	movprfx	z0, z1
**	fsub	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_m_untied, svfloat32_t,
		z0 = svsub_f32_m (p0, z1, z2),
		z0 = svsub_m (p0, z1, z2))

/*
** sub_w0_f32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	fsub	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f32_m_tied1, svfloat32_t, float,
		 z0 = svsub_n_f32_m (p0, z0, x0),
		 z0 = svsub_m (p0, z0, x0))

/*
** sub_w0_f32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	fsub	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f32_m_untied, svfloat32_t, float,
		 z0 = svsub_n_f32_m (p0, z1, x0),
		 z0 = svsub_m (p0, z1, x0))

/*
** sub_s0_f32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	fsub	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_s0_f32_m_tied1, svfloat32_t, float,
		 z1 = svsub_n_f32_m (p0, z1, d0),
		 z1 = svsub_m (p0, z1, d0))

/*
** sub_s0_f32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	fsub	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_s0_f32_m_untied, svfloat32_t, float,
		 z1 = svsub_n_f32_m (p0, z2, d0),
		 z1 = svsub_m (p0, z2, d0))

/*
** sub_1_f32_m_tied1:
**	fsub	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f32_m_tied1, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z0, 1),
		z0 = svsub_m (p0, z0, 1))

/*
** sub_1_f32_m_untied:
**	movprfx	z0, z1
**	fsub	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f32_m_untied, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z1, 1),
		z0 = svsub_m (p0, z1, 1))

/*
** sub_0p5_f32_m_tied1:
**	fsub	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f32_m_tied1, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z0, 0.5),
		z0 = svsub_m (p0, z0, 0.5))

/*
** sub_0p5_f32_m_untied:
**	movprfx	z0, z1
**	fsub	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f32_m_untied, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z1, 0.5),
		z0 = svsub_m (p0, z1, 0.5))

/*
** sub_m1_f32_m_tied1:
**	fadd	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f32_m_tied1, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z0, -1),
		z0 = svsub_m (p0, z0, -1))

/*
** sub_m1_f32_m_untied:
**	movprfx	z0, z1
**	fadd	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f32_m_untied, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z1, -1),
		z0 = svsub_m (p0, z1, -1))

/*
** sub_m0p5_f32_m_tied1:
**	fadd	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f32_m_tied1, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z0, -0.5),
		z0 = svsub_m (p0, z0, -0.5))

/*
** sub_m0p5_f32_m_untied:
**	movprfx	z0, z1
**	fadd	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f32_m_untied, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z1, -0.5),
		z0 = svsub_m (p0, z1, -0.5))

/*
** sub_m2_f32_m:
**	fmov	(z[0-9]+\.s), #2\.0(e\+0)?
**	fadd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (sub_m2_f32_m, svfloat32_t,
		z0 = svsub_n_f32_m (p0, z0, -2),
		z0 = svsub_m (p0, z0, -2))

/*
** sub_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fsub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_z_tied1, svfloat32_t,
		z0 = svsub_f32_z (p0, z0, z1),
		z0 = svsub_z (p0, z0, z1))

/*
** sub_f32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	fsubr	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_z_tied2, svfloat32_t,
		z1 = svsub_f32_z (p0, z0, z1),
		z1 = svsub_z (p0, z0, z1))

/*
** sub_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fsub	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_z_untied, svfloat32_t,
		z0 = svsub_f32_z (p0, z1, z2),
		z0 = svsub_z (p0, z1, z2))

/*
** sub_w0_f32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	fsub	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f32_z_tied1, svfloat32_t, float,
		 z0 = svsub_n_f32_z (p0, z0, x0),
		 z0 = svsub_z (p0, z0, x0))

/*
** sub_w0_f32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z1\.s
**	fsub	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f32_z_untied, svfloat32_t, float,
		 z0 = svsub_n_f32_z (p0, z1, x0),
		 z0 = svsub_z (p0, z1, x0))

/*
** sub_s0_f32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	fsub	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_s0_f32_z_tied1, svfloat32_t, float,
		 z1 = svsub_n_f32_z (p0, z1, d0),
		 z1 = svsub_z (p0, z1, d0))

/*
** sub_s0_f32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	fsub	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_s0_f32_z_untied, svfloat32_t, float,
		 z1 = svsub_n_f32_z (p0, z1, d0),
		 z1 = svsub_z (p0, z1, d0))

/*
** sub_1_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fsub	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f32_z_tied1, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z0, 1),
		z0 = svsub_z (p0, z0, 1))

/*
** sub_1_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fsub	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f32_z_untied, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z1, 1),
		z0 = svsub_z (p0, z1, 1))

/*
** sub_0p5_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fsub	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f32_z_tied1, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z0, 0.5),
		z0 = svsub_z (p0, z0, 0.5))

/*
** sub_0p5_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fsub	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f32_z_untied, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z1, 0.5),
		z0 = svsub_z (p0, z1, 0.5))

/*
** sub_m1_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fadd	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f32_z_tied1, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z0, -1),
		z0 = svsub_z (p0, z0, -1))

/*
** sub_m1_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fadd	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f32_z_untied, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z1, -1),
		z0 = svsub_z (p0, z1, -1))

/*
** sub_m0p5_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fadd	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f32_z_tied1, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z0, -0.5),
		z0 = svsub_z (p0, z0, -0.5))

/*
** sub_m0p5_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fadd	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f32_z_untied, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z1, -0.5),
		z0 = svsub_z (p0, z1, -0.5))

/*
** sub_m2_f32_z:
**	fmov	(z[0-9]+\.s), #2\.0(e\+0)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fadd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (sub_m2_f32_z, svfloat32_t,
		z0 = svsub_n_f32_z (p0, z0, -2),
		z0 = svsub_z (p0, z0, -2))

/*
** sub_f32_x_tied1:
**	fsub	z0\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_x_tied1, svfloat32_t,
		z0 = svsub_f32_x (p0, z0, z1),
		z0 = svsub_x (p0, z0, z1))

/*
** sub_f32_x_tied2:
**	fsub	z1\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_x_tied2, svfloat32_t,
		z1 = svsub_f32_x (p0, z0, z1),
		z1 = svsub_x (p0, z0, z1))

/*
** sub_f32_x_untied:
**	fsub	z2\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (sub_f32_x_untied, svfloat32_t,
		z2 = svsub_f32_x (p0, z0, z1),
		z2 = svsub_x (p0, z0, z1))

/*
** sub_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fsub	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svsub_n_f32_x (p0, z0, x0),
		 z0 = svsub_x (p0, z0, x0))

/*
** sub_w0_f32_x_untied:
**	mov	(z[0-9]+\.s), w0
**	fsub	z1\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svsub_n_f32_x (p0, z0, x0),
		 z1 = svsub_x (p0, z0, x0))

/*
** sub_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fsub	z1\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svsub_n_f32_x (p0, z1, d0),
		 z1 = svsub_x (p0, z1, d0))

/*
** sub_s0_f32_x_untied:
**	mov	(z[0-9]+\.s), s0
**	fsub	z2\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svsub_n_f32_x (p0, z1, d0),
		 z2 = svsub_x (p0, z1, d0))

/*
** sub_1_f32_x_tied1:
**	fsub	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z0, 1),
		z0 = svsub_x (p0, z0, 1))

/*
** sub_1_f32_x_untied:
**	movprfx	z0, z1
**	fsub	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z1, 1),
		z0 = svsub_x (p0, z1, 1))

/*
** sub_0p5_f32_x_tied1:
**	fsub	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z0, 0.5),
		z0 = svsub_x (p0, z0, 0.5))

/*
** sub_0p5_f32_x_untied:
**	movprfx	z0, z1
**	fsub	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z1, 0.5),
		z0 = svsub_x (p0, z1, 0.5))

/*
** sub_m1_f32_x_tied1:
**	fadd	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z0, -1),
		z0 = svsub_x (p0, z0, -1))

/*
** sub_m1_f32_x_untied:
**	movprfx	z0, z1
**	fadd	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z1, -1),
		z0 = svsub_x (p0, z1, -1))

/*
** sub_m0p5_f32_x_tied1:
**	fadd	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z0, -0.5),
		z0 = svsub_x (p0, z0, -0.5))

/*
** sub_m0p5_f32_x_untied:
**	movprfx	z0, z1
**	fadd	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z1, -0.5),
		z0 = svsub_x (p0, z1, -0.5))

/*
** sub_2_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-2\.0(e\+0)?
**	fadd	z0\.s, (z0\.s, \1|\1, z0\.s)
**	ret
*/
TEST_UNIFORM_Z (sub_2_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z0, 2),
		z0 = svsub_x (p0, z0, 2))

/*
** sub_2_f32_x_untied:
**	fmov	(z[0-9]+\.s), #-2\.0(e\+0)?
**	fadd	z0\.s, (z1\.s, \1|\1, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (sub_2_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (p0, z1, 2),
		z0 = svsub_x (p0, z1, 2))

/*
** ptrue_sub_f32_x_tied1:
**	fsub	z0\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_f32_x_tied1, svfloat32_t,
		z0 = svsub_f32_x (svptrue_b32 (), z0, z1),
		z0 = svsub_x (svptrue_b32 (), z0, z1))

/*
** ptrue_sub_f32_x_tied2:
**	fsub	z1\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_f32_x_tied2, svfloat32_t,
		z1 = svsub_f32_x (svptrue_b32 (), z0, z1),
		z1 = svsub_x (svptrue_b32 (), z0, z1))

/*
** ptrue_sub_f32_x_untied:
**	fsub	z2\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_f32_x_untied, svfloat32_t,
		z2 = svsub_f32_x (svptrue_b32 (), z0, z1),
		z2 = svsub_x (svptrue_b32 (), z0, z1))

/*
** ptrue_sub_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fsub	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svsub_n_f32_x (svptrue_b32 (), z0, x0),
		 z0 = svsub_x (svptrue_b32 (), z0, x0))

/*
** ptrue_sub_w0_f32_x_untied:
**	mov	(z[0-9]+\.s), w0
**	fsub	z1\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svsub_n_f32_x (svptrue_b32 (), z0, x0),
		 z1 = svsub_x (svptrue_b32 (), z0, x0))

/*
** ptrue_sub_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fsub	z1\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svsub_n_f32_x (svptrue_b32 (), z1, d0),
		 z1 = svsub_x (svptrue_b32 (), z1, d0))

/*
** ptrue_sub_s0_f32_x_untied:
**	mov	(z[0-9]+\.s), s0
**	fsub	z2\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svsub_n_f32_x (svptrue_b32 (), z1, d0),
		 z2 = svsub_x (svptrue_b32 (), z1, d0))

/*
** ptrue_sub_1_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fsub	z0\.s, \1/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_1_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z0, 1),
		z0 = svsub_x (svptrue_b32 (), z0, 1))

/*
** ptrue_sub_1_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.s, \1/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_1_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z1, 1),
		z0 = svsub_x (svptrue_b32 (), z1, 1))

/*
** ptrue_sub_0p5_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fsub	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_0p5_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z0, 0.5),
		z0 = svsub_x (svptrue_b32 (), z0, 0.5))

/*
** ptrue_sub_0p5_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_0p5_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z1, 0.5),
		z0 = svsub_x (svptrue_b32 (), z1, 0.5))

/*
** ptrue_sub_m1_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fadd	z0\.s, \1/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m1_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z0, -1),
		z0 = svsub_x (svptrue_b32 (), z0, -1))

/*
** ptrue_sub_m1_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.s, \1/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m1_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z1, -1),
		z0 = svsub_x (svptrue_b32 (), z1, -1))

/*
** ptrue_sub_m0p5_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fadd	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m0p5_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z0, -0.5),
		z0 = svsub_x (svptrue_b32 (), z0, -0.5))

/*
** ptrue_sub_m0p5_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m0p5_f32_x_untied, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z1, -0.5),
		z0 = svsub_x (svptrue_b32 (), z1, -0.5))

/*
** ptrue_sub_2_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-2\.0(e\+0)?
**	fadd	z0\.s, (z0\.s, \1|\1, z0\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_2_f32_x_tied1, svfloat32_t,
		z0 = svsub_n_f32_x (svptrue_b32 (), z0, 2),
		z0 = svsub_x (svptrue_b32 (), z0, 2))

/*
** ptrue_sub_2_f32_x_untied:
**	fmov	(z[0-9]+\.s), #-2\.0(e\+0)?
**	fadd	z1\.s, (z1\.s, \1|\1, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_2_f32_x_untied, svfloat32_t,
		z1 = svsub_n_f32_x (svptrue_b32 (), z1, 2),
		z1 = svsub_x (svptrue_b32 (), z1, 2))

/*
** ptrue_b16_sub_f32_x_tied1:
**	fsub	z0\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_sub_f32_x_tied1, svfloat32_t,
		z0 = svsub_f32_x (svptrue_b16 (), z0, z1),
		z0 = svsub_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_sub_f32_x_tied2:
**	fsub	z1\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_sub_f32_x_tied2, svfloat32_t,
		z1 = svsub_f32_x (svptrue_b16 (), z0, z1),
		z1 = svsub_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_sub_f32_x_untied:
**	fsub	z2\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_sub_f32_x_untied, svfloat32_t,
		z2 = svsub_f32_x (svptrue_b16 (), z0, z1),
		z2 = svsub_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b8_sub_f32_x_tied1:
**	fsub	z0\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_sub_f32_x_tied1, svfloat32_t,
		z0 = svsub_f32_x (svptrue_b8 (), z0, z1),
		z0 = svsub_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_sub_f32_x_tied2:
**	fsub	z1\.s, (z1\.s, z0\.s|z0\.s, z1\.s)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_sub_f32_x_tied2, svfloat32_t,
		z1 = svsub_f32_x (svptrue_b8 (), z0, z1),
		z1 = svsub_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_sub_f32_x_untied:
**	fsub	z2\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_sub_f32_x_untied, svfloat32_t,
		z2 = svsub_f32_x (svptrue_b8 (), z0, z1),
		z2 = svsub_x (svptrue_b8 (), z0, z1))
