/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** sub_f16_m_tied1:
**	fsub	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_m_tied1, svfloat16_t,
		z0 = svsub_f16_m (p0, z0, z1),
		z0 = svsub_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (sub_f16_m_tied2, svfloat16_t,
		z1 = svsub_f16_m (p0, z0, z1),
		z1 = svsub_m (p0, z0, z1))

/*
** sub_f16_m_untied:
**	movprfx	z0, z1
**	fsub	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_m_untied, svfloat16_t,
		z0 = svsub_f16_m (p0, z1, z2),
		z0 = svsub_m (p0, z1, z2))

/*
** sub_w0_f16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	fsub	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f16_m_tied1, svfloat16_t, __fp16,
		 z0 = svsub_n_f16_m (p0, z0, x0),
		 z0 = svsub_m (p0, z0, x0))

/*
** sub_w0_f16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	fsub	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f16_m_untied, svfloat16_t, __fp16,
		 z0 = svsub_n_f16_m (p0, z1, x0),
		 z0 = svsub_m (p0, z1, x0))

/*
** sub_h0_f16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	fsub	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_h0_f16_m_tied1, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_m (p0, z1, d0),
		 z1 = svsub_m (p0, z1, d0))

/*
** sub_h0_f16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z2
**	fsub	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_h0_f16_m_untied, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_m (p0, z2, d0),
		 z1 = svsub_m (p0, z2, d0))

/*
** sub_1_f16_m_tied1:
**	fsub	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f16_m_tied1, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z0, 1),
		z0 = svsub_m (p0, z0, 1))

/*
** sub_1_f16_m_untied:
**	movprfx	z0, z1
**	fsub	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f16_m_untied, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z1, 1),
		z0 = svsub_m (p0, z1, 1))

/*
** sub_0p5_f16_m_tied1:
**	fsub	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f16_m_tied1, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z0, 0.5),
		z0 = svsub_m (p0, z0, 0.5))

/*
** sub_0p5_f16_m_untied:
**	movprfx	z0, z1
**	fsub	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f16_m_untied, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z1, 0.5),
		z0 = svsub_m (p0, z1, 0.5))

/*
** sub_m1_f16_m_tied1:
**	fadd	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f16_m_tied1, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z0, -1),
		z0 = svsub_m (p0, z0, -1))

/*
** sub_m1_f16_m_untied:
**	movprfx	z0, z1
**	fadd	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f16_m_untied, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z1, -1),
		z0 = svsub_m (p0, z1, -1))

/*
** sub_m0p5_f16_m_tied1:
**	fadd	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f16_m_tied1, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z0, -0.5),
		z0 = svsub_m (p0, z0, -0.5))

/*
** sub_m0p5_f16_m_untied:
**	movprfx	z0, z1
**	fadd	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f16_m_untied, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z1, -0.5),
		z0 = svsub_m (p0, z1, -0.5))

/*
** sub_m2_f16_m:
**	fmov	(z[0-9]+\.h), #2\.0(e\+0)?
**	fadd	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (sub_m2_f16_m, svfloat16_t,
		z0 = svsub_n_f16_m (p0, z0, -2),
		z0 = svsub_m (p0, z0, -2))

/*
** sub_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fsub	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_z_tied1, svfloat16_t,
		z0 = svsub_f16_z (p0, z0, z1),
		z0 = svsub_z (p0, z0, z1))

/*
** sub_f16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	fsubr	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_z_tied2, svfloat16_t,
		z1 = svsub_f16_z (p0, z0, z1),
		z1 = svsub_z (p0, z0, z1))

/*
** sub_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fsub	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_z_untied, svfloat16_t,
		z0 = svsub_f16_z (p0, z1, z2),
		z0 = svsub_z (p0, z1, z2))

/*
** sub_w0_f16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	fsub	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f16_z_tied1, svfloat16_t, __fp16,
		 z0 = svsub_n_f16_z (p0, z0, x0),
		 z0 = svsub_z (p0, z0, x0))

/*
** sub_w0_f16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z1\.h
**	fsub	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f16_z_untied, svfloat16_t, __fp16,
		 z0 = svsub_n_f16_z (p0, z1, x0),
		 z0 = svsub_z (p0, z1, x0))

/*
** sub_h0_f16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	fsub	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_h0_f16_z_tied1, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_z (p0, z1, d0),
		 z1 = svsub_z (p0, z1, d0))

/*
** sub_h0_f16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z2\.h
**	fsub	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_h0_f16_z_untied, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_z (p0, z2, d0),
		 z1 = svsub_z (p0, z2, d0))

/*
** sub_1_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fsub	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f16_z_tied1, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z0, 1),
		z0 = svsub_z (p0, z0, 1))

/*
** sub_1_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fsub	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f16_z_untied, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z1, 1),
		z0 = svsub_z (p0, z1, 1))

/*
** sub_0p5_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fsub	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f16_z_tied1, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z0, 0.5),
		z0 = svsub_z (p0, z0, 0.5))

/*
** sub_0p5_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fsub	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f16_z_untied, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z1, 0.5),
		z0 = svsub_z (p0, z1, 0.5))

/*
** sub_m1_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fadd	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f16_z_tied1, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z0, -1),
		z0 = svsub_z (p0, z0, -1))

/*
** sub_m1_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fadd	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f16_z_untied, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z1, -1),
		z0 = svsub_z (p0, z1, -1))

/*
** sub_m0p5_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fadd	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f16_z_tied1, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z0, -0.5),
		z0 = svsub_z (p0, z0, -0.5))

/*
** sub_m0p5_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fadd	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f16_z_untied, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z1, -0.5),
		z0 = svsub_z (p0, z1, -0.5))

/*
** sub_m2_f16_z:
**	fmov	(z[0-9]+\.h), #2\.0(e\+0)?
**	movprfx	z0\.h, p0/z, z0\.h
**	fadd	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (sub_m2_f16_z, svfloat16_t,
		z0 = svsub_n_f16_z (p0, z0, -2),
		z0 = svsub_z (p0, z0, -2))

/*
** sub_f16_x_tied1:
**	fsub	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_x_tied1, svfloat16_t,
		z0 = svsub_f16_x (p0, z0, z1),
		z0 = svsub_x (p0, z0, z1))

/*
** sub_f16_x_tied2:
**	fsubr	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_x_tied2, svfloat16_t,
		z1 = svsub_f16_x (p0, z0, z1),
		z1 = svsub_x (p0, z0, z1))

/*
** sub_f16_x_untied:
**	movprfx	z2, z0
**	fsub	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (sub_f16_x_untied, svfloat16_t,
		z2 = svsub_f16_x (p0, z0, z1),
		z2 = svsub_x (p0, z0, z1))

/*
** sub_w0_f16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	fsub	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f16_x_tied1, svfloat16_t, __fp16,
		 z0 = svsub_n_f16_x (p0, z0, x0),
		 z0 = svsub_x (p0, z0, x0))

/*
** sub_w0_f16_x_untied:
**	mov	z1\.h, w0
**	fsubr	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_f16_x_untied, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_x (p0, z0, x0),
		 z1 = svsub_x (p0, z0, x0))

/*
** sub_h0_f16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	fsub	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_h0_f16_x_tied1, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_x (p0, z1, d0),
		 z1 = svsub_x (p0, z1, d0))

/*
** sub_h0_f16_x_untied:
**	mov	z2\.h, h0
**	fsubr	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (sub_h0_f16_x_untied, svfloat16_t, __fp16,
		 z2 = svsub_n_f16_x (p0, z1, d0),
		 z2 = svsub_x (p0, z1, d0))

/*
** sub_1_f16_x_tied1:
**	fsub	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z0, 1),
		z0 = svsub_x (p0, z0, 1))

/*
** sub_1_f16_x_untied:
**	movprfx	z0, z1
**	fsub	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_1_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z1, 1),
		z0 = svsub_x (p0, z1, 1))

/*
** sub_0p5_f16_x_tied1:
**	fsub	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z0, 0.5),
		z0 = svsub_x (p0, z0, 0.5))

/*
** sub_0p5_f16_x_untied:
**	movprfx	z0, z1
**	fsub	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_0p5_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z1, 0.5),
		z0 = svsub_x (p0, z1, 0.5))

/*
** sub_m1_f16_x_tied1:
**	fadd	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z0, -1),
		z0 = svsub_x (p0, z0, -1))

/*
** sub_m1_f16_x_untied:
**	movprfx	z0, z1
**	fadd	z0\.h, p0/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (sub_m1_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z1, -1),
		z0 = svsub_x (p0, z1, -1))

/*
** sub_m0p5_f16_x_tied1:
**	fadd	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z0, -0.5),
		z0 = svsub_x (p0, z0, -0.5))

/*
** sub_m0p5_f16_x_untied:
**	movprfx	z0, z1
**	fadd	z0\.h, p0/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (sub_m0p5_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z1, -0.5),
		z0 = svsub_x (p0, z1, -0.5))

/*
** sub_2_f16_x_tied1:
**	fmov	(z[0-9]+\.h), #-2\.0(e\+0)?
**	fadd	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (sub_2_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z0, 2),
		z0 = svsub_x (p0, z0, 2))

/*
** sub_2_f16_x_untied:
**	fmov	z0\.h, #-2\.0(e\+0)?
**	fadd	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (sub_2_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (p0, z1, 2),
		z0 = svsub_x (p0, z1, 2))

/*
** ptrue_sub_f16_x_tied1:
**	fsub	z0\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_f16_x_tied1, svfloat16_t,
		z0 = svsub_f16_x (svptrue_b16 (), z0, z1),
		z0 = svsub_x (svptrue_b16 (), z0, z1))

/*
** ptrue_sub_f16_x_tied2:
**	fsub	z1\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_f16_x_tied2, svfloat16_t,
		z1 = svsub_f16_x (svptrue_b16 (), z0, z1),
		z1 = svsub_x (svptrue_b16 (), z0, z1))

/*
** ptrue_sub_f16_x_untied:
**	fsub	z2\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_f16_x_untied, svfloat16_t,
		z2 = svsub_f16_x (svptrue_b16 (), z0, z1),
		z2 = svsub_x (svptrue_b16 (), z0, z1))

/*
** ptrue_sub_w0_f16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	fsub	z0\.h, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_w0_f16_x_tied1, svfloat16_t, __fp16,
		 z0 = svsub_n_f16_x (svptrue_b16 (), z0, x0),
		 z0 = svsub_x (svptrue_b16 (), z0, x0))

/*
** ptrue_sub_w0_f16_x_untied:
**	mov	(z[0-9]+\.h), w0
**	fsub	z1\.h, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_w0_f16_x_untied, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_x (svptrue_b16 (), z0, x0),
		 z1 = svsub_x (svptrue_b16 (), z0, x0))

/*
** ptrue_sub_h0_f16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	fsub	z1\.h, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_h0_f16_x_tied1, svfloat16_t, __fp16,
		 z1 = svsub_n_f16_x (svptrue_b16 (), z1, d0),
		 z1 = svsub_x (svptrue_b16 (), z1, d0))

/*
** ptrue_sub_h0_f16_x_untied:
**	mov	(z[0-9]+\.h), h0
**	fsub	z2\.h, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (ptrue_sub_h0_f16_x_untied, svfloat16_t, __fp16,
		 z2 = svsub_n_f16_x (svptrue_b16 (), z1, d0),
		 z2 = svsub_x (svptrue_b16 (), z1, d0))

/*
** ptrue_sub_1_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fsub	z0\.h, \1/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_1_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z0, 1),
		z0 = svsub_x (svptrue_b16 (), z0, 1))

/*
** ptrue_sub_1_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.h, \1/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_1_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z1, 1),
		z0 = svsub_x (svptrue_b16 (), z1, 1))

/*
** ptrue_sub_0p5_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fsub	z0\.h, \1/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_0p5_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z0, 0.5),
		z0 = svsub_x (svptrue_b16 (), z0, 0.5))

/*
** ptrue_sub_0p5_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z0, z1
**	fsub	z0\.h, \1/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_0p5_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z1, 0.5),
		z0 = svsub_x (svptrue_b16 (), z1, 0.5))

/*
** ptrue_sub_m1_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fadd	z0\.h, \1/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m1_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z0, -1),
		z0 = svsub_x (svptrue_b16 (), z0, -1))

/*
** ptrue_sub_m1_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.h, \1/m, z0\.h, #1\.0
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m1_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z1, -1),
		z0 = svsub_x (svptrue_b16 (), z1, -1))

/*
** ptrue_sub_m0p5_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fadd	z0\.h, \1/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m0p5_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z0, -0.5),
		z0 = svsub_x (svptrue_b16 (), z0, -0.5))

/*
** ptrue_sub_m0p5_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z0, z1
**	fadd	z0\.h, \1/m, z0\.h, #0\.5
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_m0p5_f16_x_untied, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z1, -0.5),
		z0 = svsub_x (svptrue_b16 (), z1, -0.5))

/*
** ptrue_sub_2_f16_x_tied1:
**	fmov	(z[0-9]+\.h), #-2\.0(e\+0)?
**	fadd	z0\.h, (z0\.h, \1|\1, z0\.h)
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_2_f16_x_tied1, svfloat16_t,
		z0 = svsub_n_f16_x (svptrue_b16 (), z0, 2),
		z0 = svsub_x (svptrue_b16 (), z0, 2))

/*
** ptrue_sub_2_f16_x_untied:
**	fmov	(z[0-9]+\.h), #-2\.0(e\+0)?
**	fadd	z1\.h, (z1\.h, \1|\1, z1\.h)
**	ret
*/
TEST_UNIFORM_Z (ptrue_sub_2_f16_x_untied, svfloat16_t,
		z1 = svsub_n_f16_x (svptrue_b16 (), z1, 2),
		z1 = svsub_x (svptrue_b16 (), z1, 2))

/*
** ptrue_b8_sub_f16_x_tied1:
**	fsub	z0\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_sub_f16_x_tied1, svfloat16_t,
		z0 = svsub_f16_x (svptrue_b8 (), z0, z1),
		z0 = svsub_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_sub_f16_x_tied2:
**	fsub	z1\.h, (z1\.h, z0\.h|z0\.h, z1\.h)
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_sub_f16_x_tied2, svfloat16_t,
		z1 = svsub_f16_x (svptrue_b8 (), z0, z1),
		z1 = svsub_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_sub_f16_x_untied:
**	fsub	z2\.h, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_sub_f16_x_untied, svfloat16_t,
		z2 = svsub_f16_x (svptrue_b8 (), z0, z1),
		z2 = svsub_x (svptrue_b8 (), z0, z1))
