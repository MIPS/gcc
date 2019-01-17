/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** divr_f32_m_tied1:
**	fdivr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_m_tied1, svfloat32_t,
		z0 = svdivr_f32_m (p0, z0, z1),
		z0 = svdivr_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (divr_f32_m_tied2, svfloat32_t,
		z1 = svdivr_f32_m (p0, z0, z1),
		z1 = svdivr_m (p0, z0, z1))

/*
** divr_f32_m_untied:
**	movprfx	z0, z1
**	fdivr	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_m_untied, svfloat32_t,
		z0 = svdivr_f32_m (p0, z1, z2),
		z0 = svdivr_m (p0, z1, z2))

/*
** divr_w0_f32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f32_m_tied1, svfloat32_t, float,
		 z0 = svdivr_n_f32_m (p0, z0, x0),
		 z0 = svdivr_m (p0, z0, x0))

/*
** divr_w0_f32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f32_m_untied, svfloat32_t, float,
		 z0 = svdivr_n_f32_m (p0, z1, x0),
		 z0 = svdivr_m (p0, z1, x0))

/*
** divr_s0_f32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	fdivr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_s0_f32_m_tied1, svfloat32_t, float,
		 z1 = svdivr_n_f32_m (p0, z1, d0),
		 z1 = svdivr_m (p0, z1, d0))

/*
** divr_s0_f32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	fdivr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_s0_f32_m_untied, svfloat32_t, float,
		 z1 = svdivr_n_f32_m (p0, z2, d0),
		 z1 = svdivr_m (p0, z2, d0))

/*
** divr_1_f32_m_tied1:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f32_m_tied1, svfloat32_t,
		z0 = svdivr_n_f32_m (p0, z0, 1),
		z0 = svdivr_m (p0, z0, 1))

/*
** divr_1_f32_m_untied:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	movprfx	z0, z1
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f32_m_untied, svfloat32_t,
		z0 = svdivr_n_f32_m (p0, z1, 1),
		z0 = svdivr_m (p0, z1, 1))

/*
** divr_0p5_f32_m_tied1:
**	fmov	(z[0-9]+\.s), #5.0(e\-1)?
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f32_m_tied1, svfloat32_t,
		z0 = svdivr_n_f32_m (p0, z0, 0.5),
		z0 = svdivr_m (p0, z0, 0.5))

/*
** divr_0p5_f32_m_untied:
**	fmov	(z[0-9]+\.s), #5.0(e\-1)?
**	movprfx	z0, z1
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f32_m_untied, svfloat32_t,
		z0 = svdivr_n_f32_m (p0, z1, 0.5),
		z0 = svdivr_m (p0, z1, 0.5))


/*
** divr_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fdivr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_z_tied1, svfloat32_t,
		z0 = svdivr_f32_z (p0, z0, z1),
		z0 = svdivr_z (p0, z0, z1))

/*
** divr_f32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	fdiv	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_z_tied2, svfloat32_t,
		z1 = svdivr_f32_z (p0, z0, z1),
		z1 = svdivr_z (p0, z0, z1))

/*
** divr_f32_z_untied:
**	movprfx	z0\.s, p0/z, z2\.s
**	fdiv	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_z_untied, svfloat32_t,
		z0 = svdivr_f32_z (p0, z1, z2),
		z0 = svdivr_z (p0, z1, z2))

/*
** divr_w0_f32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f32_z_tied1, svfloat32_t, float,
		 z0 = svdivr_n_f32_z (p0, z0, x0),
		 z0 = svdivr_z (p0, z0, x0))

/*
** divr_w0_f32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, \1
**	fdiv	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f32_z_untied, svfloat32_t, float,
		 z0 = svdivr_n_f32_z (p0, z1, x0),
		 z0 = svdivr_z (p0, z1, x0))

/*
** divr_s0_f32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	fdivr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_s0_f32_z_tied1, svfloat32_t, float,
		 z1 = svdivr_n_f32_z (p0, z1, d0),
		 z1 = svdivr_z (p0, z1, d0))

/*
** divr_s0_f32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, \1
**	fdiv	z1\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (divr_s0_f32_z_untied, svfloat32_t, float,
		 z1 = svdivr_n_f32_z (p0, z2, d0),
		 z1 = svdivr_z (p0, z2, d0))

/*
** divr_1_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f32_z_tied1, svfloat32_t,
		z0 = svdivr_n_f32_z (p0, z0, 1),
		z0 = svdivr_z (p0, z0, 1))

/*
** divr_0p5_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #5.0(e\-1)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f32_z_tied1, svfloat32_t,
		z0 = svdivr_n_f32_z (p0, z0, 0.5),
		z0 = svdivr_z (p0, z0, 0.5))

/*
** divr_0p5_f32_z_untied:
**	fmov	(z[0-9]+\.s), #5.0(e\-1)?
**	movprfx	z0\.s, p0/z, \1
**	fdiv	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f32_z_untied, svfloat32_t,
		z0 = svdivr_n_f32_z (p0, z1, 0.5),
		z0 = svdivr_z (p0, z1, 0.5))

/*
** divr_f32_x_tied1:
**	fdivr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_x_tied1, svfloat32_t,
		z0 = svdivr_f32_x (p0, z0, z1),
		z0 = svdivr_x (p0, z0, z1))

/*
** divr_f32_x_tied2:
**	fdiv	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_x_tied2, svfloat32_t,
		z1 = svdivr_f32_x (p0, z0, z1),
		z1 = svdivr_x (p0, z0, z1))

/*
** divr_f32_x_untied:
**	movprfx	z2, z1
**	fdiv	z2\.s, p0/m, z2\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (divr_f32_x_untied, svfloat32_t,
		z2 = svdivr_f32_x (p0, z0, z1),
		z2 = svdivr_x (p0, z0, z1))

/*
** divr_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svdivr_n_f32_x (p0, z0, x0),
		 z0 = svdivr_x (p0, z0, x0))

/*
** divr_w0_f32_x_untied:
**	mov	z1\.s, w0
**	fdiv	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svdivr_n_f32_x (p0, z0, x0),
		 z1 = svdivr_x (p0, z0, x0))

/*
** divr_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fdivr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svdivr_n_f32_x (p0, z1, d0),
		 z1 = svdivr_x (p0, z1, d0))

/*
** divr_s0_f32_x_untied:
**	mov	z2\.s, s0
**	fdiv	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (divr_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svdivr_n_f32_x (p0, z1, d0),
		 z2 = svdivr_x (p0, z1, d0))

/*
** divr_1_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #1\.0(e\+0)?
**	fdivr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f32_x_tied1, svfloat32_t,
		z0 = svdivr_n_f32_x (p0, z0, 1),
		z0 = svdivr_x (p0, z0, 1))

/*
** divr_1_f32_x_untied:
**	fmov	z0\.s, #1\.0(e\+0)?
**	fdiv	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (divr_1_f32_x_untied, svfloat32_t,
		z0 = svdivr_n_f32_x (p0, z1, 1),
		z0 = svdivr_x (p0, z1, 1))

/*
** ptrue_divr_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fdivr	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f32_x_tied1, svfloat32_t,
		z0 = svdivr_f32_x (svptrue_b32 (), z0, z1),
		z0 = svdivr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_divr_f32_x_tied2:
**	ptrue	(p[0-7])\.s[^\n]*
**	fdiv	z1\.s, \1/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f32_x_tied2, svfloat32_t,
		z1 = svdivr_f32_x (svptrue_b32 (), z0, z1),
		z1 = svdivr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_divr_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z2, z1
**	fdiv	z2\.s, \1/m, z2\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f32_x_untied, svfloat32_t,
		z2 = svdivr_f32_x (svptrue_b32 (), z0, z1),
		z2 = svdivr_x (svptrue_b32 (), z0, z1))

/*
** ptrue_divr_w0_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	(z[0-9]+\.s), w0
**	fdivr	z0\.s, \1/m, z0\.s, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svdivr_n_f32_x (svptrue_b32 (), z0, x0),
		 z0 = svdivr_x (svptrue_b32 (), z0, x0))

/*
** ptrue_divr_w0_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	z1\.s, w0
**	fdiv	z1\.s, \1/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svdivr_n_f32_x (svptrue_b32 (), z0, x0),
		 z1 = svdivr_x (svptrue_b32 (), z0, x0))

/*
** ptrue_divr_s0_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	(z[0-9]+\.s), s0
**	fdivr	z1\.s, \1/m, z1\.s, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svdivr_n_f32_x (svptrue_b32 (), z1, d0),
		 z1 = svdivr_x (svptrue_b32 (), z1, d0))

/*
** ptrue_divr_s0_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	z2\.s, s0
**	fdiv	z2\.s, \1/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svdivr_n_f32_x (svptrue_b32 (), z1, d0),
		 z2 = svdivr_x (svptrue_b32 (), z1, d0))

/*
** ptrue_divr_1_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	z0\.s, #1.0(e\+0)?
**	fdiv	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_1_f32_x_untied, svfloat32_t,
		z0 = svdivr_n_f32_x (svptrue_b32 (), z1, 1),
		z0 = svdivr_x (svptrue_b32 (), z1, 1))

/*
** ptrue_b8_divr_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fdivr	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f32_x_tied1, svfloat32_t,
		z0 = svdivr_f32_x (svptrue_b8 (), z0, z1),
		z0 = svdivr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_divr_f32_x_tied2:
**	ptrue	(p[0-7])\.s[^\n]*
**	fdiv	z1\.s, \1/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f32_x_tied2, svfloat32_t,
		z1 = svdivr_f32_x (svptrue_b8 (), z0, z1),
		z1 = svdivr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_divr_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z2, z1
**	fdiv	z2\.s, \1/m, z2\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f32_x_untied, svfloat32_t,
		z2 = svdivr_f32_x (svptrue_b8 (), z0, z1),
		z2 = svdivr_x (svptrue_b8 (), z0, z1))
