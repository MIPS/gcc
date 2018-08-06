/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** subr_f32_m_tied1:
**	fsubr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_m_tied1, svfloat32_t,
		z0 = svsubr_f32_m (p0, z0, z1),
		z0 = svsubr_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (subr_f32_m_tied2, svfloat32_t,
		z1 = svsubr_f32_m (p0, z0, z1),
		z1 = svsubr_m (p0, z0, z1))

/*
** subr_f32_m_untied:
**	movprfx	z0, z1
**	fsubr	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_m_untied, svfloat32_t,
		z0 = svsubr_f32_m (p0, z1, z2),
		z0 = svsubr_m (p0, z1, z2))

/*
** subr_w0_f32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_f32_m_tied1, svfloat32_t, float,
		 z0 = svsubr_n_f32_m (p0, z0, x0),
		 z0 = svsubr_m (p0, z0, x0))

/*
** subr_w0_f32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_f32_m_untied, svfloat32_t, float,
		 z0 = svsubr_n_f32_m (p0, z1, x0),
		 z0 = svsubr_m (p0, z1, x0))

/*
** subr_s0_f32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	fsubr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_f32_m_tied1, svfloat32_t, float,
		 z1 = svsubr_n_f32_m (p0, z1, d0),
		 z1 = svsubr_m (p0, z1, d0))

/*
** subr_s0_f32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	fsubr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_f32_m_untied, svfloat32_t, float,
		 z1 = svsubr_n_f32_m (p0, z2, d0),
		 z1 = svsubr_m (p0, z2, d0))

/*
** subr_1_f32_m_tied1:
**	fsubr	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f32_m_tied1, svfloat32_t,
		z0 = svsubr_n_f32_m (p0, z0, 1),
		z0 = svsubr_m (p0, z0, 1))

/*
** subr_1_f32_m_untied:
**	movprfx	z0, z1
**	fsubr	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f32_m_untied, svfloat32_t,
		z0 = svsubr_n_f32_m (p0, z1, 1),
		z0 = svsubr_m (p0, z1, 1))

/*
** subr_0p5_f32_m_tied1:
**	fsubr	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f32_m_tied1, svfloat32_t,
		z0 = svsubr_n_f32_m (p0, z0, 0.5),
		z0 = svsubr_m (p0, z0, 0.5))

/*
** subr_0p5_f32_m_untied:
**	movprfx	z0, z1
**	fsubr	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f32_m_untied, svfloat32_t,
		z0 = svsubr_n_f32_m (p0, z1, 0.5),
		z0 = svsubr_m (p0, z1, 0.5))

/*
** subr_m1_f32_m_tied1:
**	fmov	(z[0-9]+\.s), #-1\.0(e\+0)?
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f32_m_tied1, svfloat32_t,
		z0 = svsubr_n_f32_m (p0, z0, -1),
		z0 = svsubr_m (p0, z0, -1))

/*
** subr_m1_f32_m_untied:
**	fmov	(z[0-9]+\.s), #-1\.0(e\+0)?
**	movprfx	z0, z1
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f32_m_untied, svfloat32_t,
		z0 = svsubr_n_f32_m (p0, z1, -1),
		z0 = svsubr_m (p0, z1, -1))

/*
** subr_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fsubr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_z_tied1, svfloat32_t,
		z0 = svsubr_f32_z (p0, z0, z1),
		z0 = svsubr_z (p0, z0, z1))

/*
** subr_f32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	fsub	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_z_tied2, svfloat32_t,
		z1 = svsubr_f32_z (p0, z0, z1),
		z1 = svsubr_z (p0, z0, z1))

/*
** subr_f32_z_untied:
**	movprfx	z0\.s, p0/z, z2\.s
**	fsub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_z_untied, svfloat32_t,
		z0 = svsubr_f32_z (p0, z1, z2),
		z0 = svsubr_z (p0, z1, z2))

/*
** subr_w0_f32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_f32_z_tied1, svfloat32_t, float,
		 z0 = svsubr_n_f32_z (p0, z0, x0),
		 z0 = svsubr_z (p0, z0, x0))

/*
** subr_w0_f32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, \1
**	fsub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_f32_z_untied, svfloat32_t, float,
		 z0 = svsubr_n_f32_z (p0, z1, x0),
		 z0 = svsubr_z (p0, z1, x0))

/*
** subr_s0_f32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	fsubr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_f32_z_tied1, svfloat32_t, float,
		 z1 = svsubr_n_f32_z (p0, z1, d0),
		 z1 = svsubr_z (p0, z1, d0))

/*
** subr_s0_f32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, \1
**	fsub	z1\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_f32_z_untied, svfloat32_t, float,
		 z1 = svsubr_n_f32_z (p0, z2, d0),
		 z1 = svsubr_z (p0, z2, d0))

/*
** subr_1_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fsubr	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f32_z_tied1, svfloat32_t,
		z0 = svsubr_n_f32_z (p0, z0, 1),
		z0 = svsubr_z (p0, z0, 1))

/*
** subr_1_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fsubr	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f32_z_untied, svfloat32_t,
		z0 = svsubr_n_f32_z (p0, z1, 1),
		z0 = svsubr_z (p0, z1, 1))

/*
** subr_0p5_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fsubr	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f32_z_tied1, svfloat32_t,
		z0 = svsubr_n_f32_z (p0, z0, 0.5),
		z0 = svsubr_z (p0, z0, 0.5))

/*
** subr_0p5_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fsubr	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f32_z_untied, svfloat32_t,
		z0 = svsubr_n_f32_z (p0, z1, 0.5),
		z0 = svsubr_z (p0, z1, 0.5))

/*
** subr_m1_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #-1\.0(e\+0)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f32_z_tied1, svfloat32_t,
		z0 = svsubr_n_f32_z (p0, z0, -1),
		z0 = svsubr_z (p0, z0, -1))

/*
** subr_m1_f32_z_untied:
**	fmov	(z[0-9]+\.s), #-1\.0(e\+0)?
**	movprfx	z0\.s, p0/z, \1
**	fsub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f32_z_untied, svfloat32_t,
		z0 = svsubr_n_f32_z (p0, z1, -1),
		z0 = svsubr_z (p0, z1, -1))

/*
** subr_f32_x_tied1:
**	fsubr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_x_tied1, svfloat32_t,
		z0 = svsubr_f32_x (p0, z0, z1),
		z0 = svsubr_x (p0, z0, z1))

/*
** subr_f32_x_tied2:
**	fsub	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_x_tied2, svfloat32_t,
		z1 = svsubr_f32_x (p0, z0, z1),
		z1 = svsubr_x (p0, z0, z1))

/*
** subr_f32_x_untied:
**	movprfx	z2, z1
**	fsub	z2\.s, p0/m, z2\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_f32_x_untied, svfloat32_t,
		z2 = svsubr_f32_x (p0, z0, z1),
		z2 = svsubr_x (p0, z0, z1))

/*
** subr_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svsubr_n_f32_x (p0, z0, x0),
		 z0 = svsubr_x (p0, z0, x0))

/*
** subr_w0_f32_x_untied:
**	mov	z1\.s, w0
**	fsub	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svsubr_n_f32_x (p0, z0, x0),
		 z1 = svsubr_x (p0, z0, x0))

/*
** subr_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fsubr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svsubr_n_f32_x (p0, z1, d0),
		 z1 = svsubr_x (p0, z1, d0))

/*
** subr_s0_f32_x_untied:
**	mov	z2\.s, s0
**	fsub	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svsubr_n_f32_x (p0, z1, d0),
		 z2 = svsubr_x (p0, z1, d0))

/*
** subr_1_f32_x_tied1:
**	fsubr	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f32_x_tied1, svfloat32_t,
		z0 = svsubr_n_f32_x (p0, z0, 1),
		z0 = svsubr_x (p0, z0, 1))

/*
** subr_1_f32_x_untied:
**	movprfx	z0, z1
**	fsubr	z0\.s, p0/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (subr_1_f32_x_untied, svfloat32_t,
		z0 = svsubr_n_f32_x (p0, z1, 1),
		z0 = svsubr_x (p0, z1, 1))

/*
** subr_0p5_f32_x_tied1:
**	fsubr	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f32_x_tied1, svfloat32_t,
		z0 = svsubr_n_f32_x (p0, z0, 0.5),
		z0 = svsubr_x (p0, z0, 0.5))

/*
** subr_0p5_f32_x_untied:
**	movprfx	z0, z1
**	fsubr	z0\.s, p0/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (subr_0p5_f32_x_untied, svfloat32_t,
		z0 = svsubr_n_f32_x (p0, z1, 0.5),
		z0 = svsubr_x (p0, z1, 0.5))

/*
** subr_m1_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-1\.0(e\+0)?
**	fsubr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f32_x_tied1, svfloat32_t,
		z0 = svsubr_n_f32_x (p0, z0, -1),
		z0 = svsubr_x (p0, z0, -1))

/*
** subr_m1_f32_x_untied:
**	fmov	z0\.s, #-1\.0(e\+0)?
**	fsub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_m1_f32_x_untied, svfloat32_t,
		z0 = svsubr_n_f32_x (p0, z1, -1),
		z0 = svsubr_x (p0, z1, -1))

/*
** ptrue_subr_f32_x_tied1:
**	fsub	z0\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_f32_x_tied1, svfloat32_t,
		z0 = svsubr_f32_x (svptrue_b32 (), z0, z1),
		z0 = svsubr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_subr_f32_x_tied2:
**	fsub	z1\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_f32_x_tied2, svfloat32_t,
		z1 = svsubr_f32_x (svptrue_b32 (), z0, z1),
		z1 = svsubr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_subr_f32_x_untied:
**	fsub	z2\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_f32_x_untied, svfloat32_t,
		z2 = svsubr_f32_x (svptrue_b32 (), z0, z1),
		z2 = svsubr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_subr_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fsub	z0\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svsubr_n_f32_x (svptrue_b32 (), z0, x0),
		 z0 = svsubr_x (svptrue_b32 (), z0, x0))

/*
** ptrue_subr_w0_f32_x_untied:
**	mov	(z[0-9]+\.s), w0
**	fsub	z1\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svsubr_n_f32_x (svptrue_b32 (), z0, x0),
		 z1 = svsubr_x (svptrue_b32 (), z0, x0))

/*
** ptrue_subr_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fsub	z1\.s, \1, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svsubr_n_f32_x (svptrue_b32 (), z1, d0),
		 z1 = svsubr_x (svptrue_b32 (), z1, d0))

/*
** ptrue_subr_s0_f32_x_untied:
**	mov	(z[0-9]+\.s), s0
**	fsub	z2\.s, \1, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_subr_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svsubr_n_f32_x (svptrue_b32 (), z1, d0),
		 z2 = svsubr_x (svptrue_b32 (), z1, d0))

/*
** ptrue_subr_1_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fsubr	z0\.s, \1/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_1_f32_x_tied1, svfloat32_t,
		z0 = svsubr_n_f32_x (svptrue_b32 (), z0, 1),
		z0 = svsubr_x (svptrue_b32 (), z0, 1))

/*
** ptrue_subr_1_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z0, z1
**	fsubr	z0\.s, \1/m, z0\.s, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_1_f32_x_untied, svfloat32_t,
		z0 = svsubr_n_f32_x (svptrue_b32 (), z1, 1),
		z0 = svsubr_x (svptrue_b32 (), z1, 1))

/*
** ptrue_subr_0p5_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fsubr	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_0p5_f32_x_tied1, svfloat32_t,
		z0 = svsubr_n_f32_x (svptrue_b32 (), z0, 0.5),
		z0 = svsubr_x (svptrue_b32 (), z0, 0.5))

/*
** ptrue_subr_0p5_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z0, z1
**	fsubr	z0\.s, \1/m, z0\.s, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_0p5_f32_x_untied, svfloat32_t,
		z0 = svsubr_n_f32_x (svptrue_b32 (), z1, 0.5),
		z0 = svsubr_x (svptrue_b32 (), z1, 0.5))

/*
** ptrue_subr_m1_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-1\.0(e\+0)?
**	fsub	z0\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_m1_f32_x_tied1, svfloat32_t,
		z0 = svsubr_n_f32_x (svptrue_b32 (), z0, -1),
		z0 = svsubr_x (svptrue_b32 (), z0, -1))

/*
** ptrue_subr_m1_f32_x_untied:
**	fmov	(z[0-9]+\.s), #-1\.0(e\+0)?
**	fsub	z1\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_subr_m1_f32_x_untied, svfloat32_t,
		z1 = svsubr_n_f32_x (svptrue_b32 (), z0, -1),
		z1 = svsubr_x (svptrue_b32 (), z0, -1))

/*
** ptrue_b16_subr_f32_x_tied1:
**	fsub	z0\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_subr_f32_x_tied1, svfloat32_t,
		z0 = svsubr_f32_x (svptrue_b16 (), z0, z1),
		z0 = svsubr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_subr_f32_x_tied2:
**	fsub	z1\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_subr_f32_x_tied2, svfloat32_t,
		z1 = svsubr_f32_x (svptrue_b16 (), z0, z1),
		z1 = svsubr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b16_subr_f32_x_untied:
**	fsub	z2\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b16_subr_f32_x_untied, svfloat32_t,
		z2 = svsubr_f32_x (svptrue_b16 (), z0, z1),
		z2 = svsubr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_b8_subr_f32_x_tied1:
**	fsub	z0\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_subr_f32_x_tied1, svfloat32_t,
		z0 = svsubr_f32_x (svptrue_b8 (), z0, z1),
		z0 = svsubr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_subr_f32_x_tied2:
**	fsub	z1\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_subr_f32_x_tied2, svfloat32_t,
		z1 = svsubr_f32_x (svptrue_b8 (), z0, z1),
		z1 = svsubr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_subr_f32_x_untied:
**	fsub	z2\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_subr_f32_x_untied, svfloat32_t,
		z2 = svsubr_f32_x (svptrue_b8 (), z0, z1),
		z2 = svsubr_x (svptrue_b8 (), z0, z1))
