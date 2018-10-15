/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** abd_f32_m_tied1:
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_m_tied1, svfloat32_t,
		z0 = svabd_f32_m (p0, z0, z1),
		z0 = svabd_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (abd_f32_m_tied2, svfloat32_t,
		z1 = svabd_f32_m (p0, z0, z1),
		z1 = svabd_m (p0, z0, z1))

/*
** abd_f32_m_untied:
**	movprfx	z0, z1
**	fabd	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_m_untied, svfloat32_t,
		z0 = svabd_f32_m (p0, z1, z2),
		z0 = svabd_m (p0, z1, z2))

/*
** abd_w0_f32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_f32_m_tied1, svfloat32_t, float,
		 z0 = svabd_n_f32_m (p0, z0, x0),
		 z0 = svabd_m (p0, z0, x0))

/*
** abd_w0_f32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_f32_m_untied, svfloat32_t, float,
		 z0 = svabd_n_f32_m (p0, z1, x0),
		 z0 = svabd_m (p0, z1, x0))

/*
** abd_s0_f32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	fabd	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_s0_f32_m_tied1, svfloat32_t, float,
		 z1 = svabd_n_f32_m (p0, z1, d0),
		 z1 = svabd_m (p0, z1, d0))

/*
** abd_s0_f32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	fabd	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_s0_f32_m_untied, svfloat32_t, float,
		 z1 = svabd_n_f32_m (p0, z2, d0),
		 z1 = svabd_m (p0, z2, d0))

/*
** abd_1_f32_m_tied1:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_1_f32_m_tied1, svfloat32_t,
		z0 = svabd_n_f32_m (p0, z0, 1),
		z0 = svabd_m (p0, z0, 1))

/*
** abd_1_f32_m_untied:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	movprfx	z0, z1
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_1_f32_m_untied, svfloat32_t,
		z0 = svabd_n_f32_m (p0, z1, 1),
		z0 = svabd_m (p0, z1, 1))

/*
** abd_f32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_z_tied1, svfloat32_t,
		z0 = svabd_f32_z (p0, z0, z1),
		z0 = svabd_z (p0, z0, z1))

/*
** abd_f32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	fabd	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_z_tied2, svfloat32_t,
		z1 = svabd_f32_z (p0, z0, z1),
		z1 = svabd_z (p0, z0, z1))

/*
** abd_f32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	fabd	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_z_untied, svfloat32_t,
		z0 = svabd_f32_z (p0, z1, z2),
		z0 = svabd_z (p0, z1, z2))

/*
** abd_w0_f32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_f32_z_tied1, svfloat32_t, float,
		 z0 = svabd_n_f32_z (p0, z0, x0),
		 z0 = svabd_z (p0, z0, x0))

/*
** abd_w0_f32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, \1
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_f32_z_untied, svfloat32_t, float,
		 z0 = svabd_n_f32_z (p0, z1, x0),
		 z0 = svabd_z (p0, z1, x0))

/*
** abd_s0_f32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	fabd	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_s0_f32_z_tied1, svfloat32_t, float,
		 z1 = svabd_n_f32_z (p0, z1, d0),
		 z1 = svabd_z (p0, z1, d0))

/*
** abd_s0_f32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, \1
**	fabd	z1\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (abd_s0_f32_z_untied, svfloat32_t, float,
		 z1 = svabd_n_f32_z (p0, z2, d0),
		 z1 = svabd_z (p0, z2, d0))

/*
** abd_1_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_1_f32_z_tied1, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z0, 1),
		z0 = svabd_z (p0, z0, 1))

/*
** abd_1_f32_z_untied:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	movprfx	z0\.s, p0/z, \1
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_1_f32_z_untied, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z1, 1),
		z0 = svabd_z (p0, z1, 1))

/*
** abd_0p5_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #5.0(e\-1)
**	movprfx	z0\.s, p0/z, z0\.s
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_0p5_f32_z_tied1, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z0, 0.5),
		z0 = svabd_z (p0, z0, 0.5))

/*
** abd_0p5_f32_z_untied:
**	fmov	(z[0-9]+\.s), #5.0(e\-1)
**	movprfx	z0\.s, p0/z, \1
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_0p5_f32_z_untied, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z1, 0.5),
		z0 = svabd_z (p0, z1, 0.5))

/*
** abd_m1_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #-1.0(e\+0)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_m1_f32_z_tied1, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z0, -1),
		z0 = svabd_z (p0, z0, -1))

/*
** abd_m1_f32_z_untied:
**	fmov	(z[0-9]+\.s), #-1.0(e\+0)?
**	movprfx	z0\.s, p0/z, \1
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_m1_f32_z_untied, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z1, -1),
		z0 = svabd_z (p0, z1, -1))

/*
** abd_m0p5_f32_z_tied1:
**	fmov	(z[0-9]+\.s), #-5.0(e\-1)
**	movprfx	z0\.s, p0/z, z0\.s
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_m0p5_f32_z_tied1, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z0, -0.5),
		z0 = svabd_z (p0, z0, -0.5))

/*
** abd_m0p5_f32_z_untied:
**	fmov	(z[0-9]+\.s), #-5.0(e\-1)?
**	movprfx	z0\.s, p0/z, \1
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_m0p5_f32_z_untied, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z1, -0.5),
		z0 = svabd_z (p0, z1, -0.5))

/*
** abd_m2_f32_z:
**	fmov	(z[0-9]+\.s), #-2.0(e\+0)?
**	movprfx	z0\.s, p0/z, z0\.s
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_m2_f32_z, svfloat32_t,
		z0 = svabd_n_f32_z (p0, z0, -2),
		z0 = svabd_z (p0, z0, -2))

/*
** abd_f32_x_tied1:
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_x_tied1, svfloat32_t,
		z0 = svabd_f32_x (p0, z0, z1),
		z0 = svabd_x (p0, z0, z1))

/*
** abd_f32_x_tied2:
**	fabd	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_x_tied2, svfloat32_t,
		z1 = svabd_f32_x (p0, z0, z1),
		z1 = svabd_x (p0, z0, z1))

/*
** abd_f32_x_untied:
**	movprfx	z2, z0
**	fabd	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_f32_x_untied, svfloat32_t,
		z2 = svabd_f32_x (p0, z0, z1),
		z2 = svabd_x (p0, z0, z1))

/*
** abd_w0_f32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svabd_n_f32_x (p0, z0, x0),
		 z0 = svabd_x (p0, z0, x0))

/*
** abd_w0_f32_x_untied:
**	mov	z1\.s, w0
**	fabd	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svabd_n_f32_x (p0, z0, x0),
		 z1 = svabd_x (p0, z0, x0))

/*
** abd_s0_f32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	fabd	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svabd_n_f32_x (p0, z1, d0),
		 z1 = svabd_x (p0, z1, d0))

/*
** abd_s0_f32_x_untied:
**	mov	z2\.s, s0
**	fabd	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (abd_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svabd_n_f32_x (p0, z1, d0),
		 z2 = svabd_x (p0, z1, d0))

/*
** abd_1_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_1_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z0, 1),
		z0 = svabd_x (p0, z0, 1))

/*
** abd_1_f32_x_untied:
**	fmov	z0\.s, #1.0(e\+0)?
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_1_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z1, 1),
		z0 = svabd_x (p0, z1, 1))

/*
** abd_0p5_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #5.0(e\-1)?
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_0p5_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z0, 0.5),
		z0 = svabd_x (p0, z0, 0.5))

/*
** abd_0p5_f32_x_untied:
**	fmov	z0\.s, #5.0(e\-1)?
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_0p5_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z1, 0.5),
		z0 = svabd_x (p0, z1, 0.5))

/*
** abd_m1_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-1.0(e\+0)?
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_m1_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z0, -1),
		z0 = svabd_x (p0, z0, -1))

/*
** abd_m1_f32_x_untied:
**	fmov	z0\.s, #-1.0(e\+0)?
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_m1_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z1, -1),
		z0 = svabd_x (p0, z1, -1))

/*
** abd_m0p5_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #-5.0(e\-1)?
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_m0p5_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z0, -0.5),
		z0 = svabd_x (p0, z0, -0.5))

/*
** abd_m0p5_f32_x_untied:
**	fmov	z0\.s, #-5.0(e\-1)?
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_m0p5_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z1, -0.5),
		z0 = svabd_x (p0, z1, -0.5))

/*
** abd_2_f32_x_tied1:
**	fmov	(z[0-9]+\.s), #2.0(e\+0)?
**	fabd	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (abd_2_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z0, 2),
		z0 = svabd_x (p0, z0, 2))

/*
** abd_2_f32_x_untied:
**	fmov	z0\.s, #2.0(e\+0)?
**	fabd	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (abd_2_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (p0, z1, 2),
		z0 = svabd_x (p0, z1, 2))

/*
** ptrue_abd_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fabd	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_f32_x_tied1, svfloat32_t,
		z0 = svabd_f32_x (svptrue_b32 (), z0, z1),
		z0 = svabd_x (svptrue_b32 (), z0, z1))

/*
** ptrue_abd_f32_x_tied2:
**	ptrue	(p[0-7])\.s[^\n]*
**	fabd	z1\.s, \1/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_f32_x_tied2, svfloat32_t,
		z1 = svabd_f32_x (svptrue_b32 (), z0, z1),
		z1 = svabd_x (svptrue_b32 (), z0, z1))

/*
** ptrue_abd_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z2, z0
**	fabd	z2\.s, \1/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_f32_x_untied, svfloat32_t,
		z2 = svabd_f32_x (svptrue_b32 (), z0, z1),
		z2 = svabd_x (svptrue_b32 (), z0, z1))

/*
** ptrue_abd_w0_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	(z[0-9]+\.s), w0
**	fabd	z0\.s, \1/m, z0\.s, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_abd_w0_f32_x_tied1, svfloat32_t, float,
		 z0 = svabd_n_f32_x (svptrue_b32 (), z0, x0),
		 z0 = svabd_x (svptrue_b32 (), z0, x0))

/*
** ptrue_abd_w0_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	z1\.s, w0
**	fabd	z1\.s, \1/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_abd_w0_f32_x_untied, svfloat32_t, float,
		 z1 = svabd_n_f32_x (svptrue_b32 (), z0, x0),
		 z1 = svabd_x (svptrue_b32 (), z0, x0))

/*
** ptrue_abd_s0_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	(z[0-9]+\.s), s0
**	fabd	z1\.s, \1/m, z1\.s, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_abd_s0_f32_x_tied1, svfloat32_t, float,
		 z1 = svabd_n_f32_x (svptrue_b32 (), z1, d0),
		 z1 = svabd_x (svptrue_b32 (), z1, d0))

/*
** ptrue_abd_s0_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	mov	z2\.s, s0
**	fabd	z2\.s, \1/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (ptrue_abd_s0_f32_x_untied, svfloat32_t, float,
		 z2 = svabd_n_f32_x (svptrue_b32 (), z1, d0),
		 z2 = svabd_x (svptrue_b32 (), z1, d0))

/*
** ptrue_abd_1_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	(z[0-9]+\.s), #1.0(e\+0)?
**	fabd	z0\.s, \1/m, z0\.s, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_1_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z0, 1),
		z0 = svabd_x (svptrue_b32 (), z0, 1))

/*
** ptrue_abd_1_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	z0\.s, #1.0(e\+0)?
**	fabd	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_1_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z1, 1),
		z0 = svabd_x (svptrue_b32 (), z1, 1))

/*
** ptrue_abd_0p5_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	(z[0-9]+\.s), #5.0(e\-1)?
**	fabd	z0\.s, \1/m, z0\.s, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_0p5_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z0, 0.5),
		z0 = svabd_x (svptrue_b32 (), z0, 0.5))

/*
** ptrue_abd_0p5_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	z0\.s, #5.0(e\-1)?
**	fabd	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_0p5_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z1, 0.5),
		z0 = svabd_x (svptrue_b32 (), z1, 0.5))

/*
** ptrue_abd_m1_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	(z[0-9]+\.s), #-1.0(e\+0)?
**	fabd	z0\.s, \1/m, z0\.s, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_m1_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z0, -1),
		z0 = svabd_x (svptrue_b32 (), z0, -1))

/*
** ptrue_abd_m1_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	z0\.s, #-1.0(e\+0)?
**	fabd	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_m1_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z1, -1),
		z0 = svabd_x (svptrue_b32 (), z1, -1))

/*
** ptrue_abd_m0p5_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	(z[0-9]+\.s), #-5.0(e\-1)?
**	fabd	z0\.s, \1/m, z0\.s, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_m0p5_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z0, -0.5),
		z0 = svabd_x (svptrue_b32 (), z0, -0.5))

/*
** ptrue_abd_m0p5_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	z0\.s, #-5.0(e\-1)?
**	fabd	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_m0p5_f32_x_untied, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z1, -0.5),
		z0 = svabd_x (svptrue_b32 (), z1, -0.5))

/*
** ptrue_abd_2_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	(z[0-9]+\.s), #2.0(e\+0)?
**	fabd	z0\.s, \1/m, z0\.s, \2
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_2_f32_x_tied1, svfloat32_t,
		z0 = svabd_n_f32_x (svptrue_b32 (), z0, 2),
		z0 = svabd_x (svptrue_b32 (), z0, 2))

/*
** ptrue_abd_2_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fmov	z1\.s, #2.0(e\+0)?
**	fabd	z1\.s, \1/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_abd_2_f32_x_untied, svfloat32_t,
		z1 = svabd_n_f32_x (svptrue_b32 (), z0, 2),
		z1 = svabd_x (svptrue_b32 (), z0, 2))

/*
** ptrue_b8_abd_f32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fabd	z0\.s, \1/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_abd_f32_x_tied1, svfloat32_t,
		z0 = svabd_f32_x (svptrue_b8 (), z0, z1),
		z0 = svabd_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_abd_f32_x_tied2:
**	ptrue	(p[0-7])\.s[^\n]*
**	fabd	z1\.s, \1/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_abd_f32_x_tied2, svfloat32_t,
		z1 = svabd_f32_x (svptrue_b8 (), z0, z1),
		z1 = svabd_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_abd_f32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	movprfx	z2, z0
**	fabd	z2\.s, \1/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_abd_f32_x_untied, svfloat32_t,
		z2 = svabd_f32_x (svptrue_b8 (), z0, z1),
		z2 = svabd_x (svptrue_b8 (), z0, z1))
