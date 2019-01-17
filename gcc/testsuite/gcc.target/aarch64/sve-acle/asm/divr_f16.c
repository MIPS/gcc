/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** divr_f16_m_tied1:
**	fdivr	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_m_tied1, svfloat16_t,
		z0 = svdivr_f16_m (p0, z0, z1),
		z0 = svdivr_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (divr_f16_m_tied2, svfloat16_t,
		z1 = svdivr_f16_m (p0, z0, z1),
		z1 = svdivr_m (p0, z0, z1))

/*
** divr_f16_m_untied:
**	movprfx	z0, z1
**	fdivr	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_m_untied, svfloat16_t,
		z0 = svdivr_f16_m (p0, z1, z2),
		z0 = svdivr_m (p0, z1, z2))

/*
** divr_w0_f16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f16_m_tied1, svfloat16_t, __fp16,
		 z0 = svdivr_n_f16_m (p0, z0, x0),
		 z0 = svdivr_m (p0, z0, x0))

/*
** divr_w0_f16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f16_m_untied, svfloat16_t, __fp16,
		 z0 = svdivr_n_f16_m (p0, z1, x0),
		 z0 = svdivr_m (p0, z1, x0))

/*
** divr_h0_f16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	fdivr	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_h0_f16_m_tied1, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_m (p0, z1, d0),
		 z1 = svdivr_m (p0, z1, d0))

/*
** divr_h0_f16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z2
**	fdivr	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_h0_f16_m_untied, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_m (p0, z2, d0),
		 z1 = svdivr_m (p0, z2, d0))

/*
** divr_1_f16_m_tied1:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f16_m_tied1, svfloat16_t,
		z0 = svdivr_n_f16_m (p0, z0, 1),
		z0 = svdivr_m (p0, z0, 1))

/*
** divr_1_f16_m_untied:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	movprfx	z0, z1
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f16_m_untied, svfloat16_t,
		z0 = svdivr_n_f16_m (p0, z1, 1),
		z0 = svdivr_m (p0, z1, 1))

/*
** divr_0p5_f16_m_tied1:
**	fmov	(z[0-9]+\.h), #5.0(e\-1)?
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f16_m_tied1, svfloat16_t,
		z0 = svdivr_n_f16_m (p0, z0, 0.5),
		z0 = svdivr_m (p0, z0, 0.5))

/*
** divr_0p5_f16_m_untied:
**	fmov	(z[0-9]+\.h), #5.0(e\-1)?
**	movprfx	z0, z1
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f16_m_untied, svfloat16_t,
		z0 = svdivr_n_f16_m (p0, z1, 0.5),
		z0 = svdivr_m (p0, z1, 0.5))


/*
** divr_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fdivr	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_z_tied1, svfloat16_t,
		z0 = svdivr_f16_z (p0, z0, z1),
		z0 = svdivr_z (p0, z0, z1))

/*
** divr_f16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	fdiv	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_z_tied2, svfloat16_t,
		z1 = svdivr_f16_z (p0, z0, z1),
		z1 = svdivr_z (p0, z0, z1))

/*
** divr_f16_z_untied:
**	movprfx	z0\.h, p0/z, z2\.h
**	fdiv	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_z_untied, svfloat16_t,
		z0 = svdivr_f16_z (p0, z1, z2),
		z0 = svdivr_z (p0, z1, z2))

/*
** divr_w0_f16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f16_z_tied1, svfloat16_t, __fp16,
		 z0 = svdivr_n_f16_z (p0, z0, x0),
		 z0 = svdivr_z (p0, z0, x0))

/*
** divr_w0_f16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, \1
**	fdiv	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f16_z_untied, svfloat16_t, __fp16,
		 z0 = svdivr_n_f16_z (p0, z1, x0),
		 z0 = svdivr_z (p0, z1, x0))

/*
** divr_h0_f16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	fdivr	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_h0_f16_z_tied1, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_z (p0, z1, d0),
		 z1 = svdivr_z (p0, z1, d0))

/*
** divr_h0_f16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, \1
**	fdiv	z1\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_ZS (divr_h0_f16_z_untied, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_z (p0, z2, d0),
		 z1 = svdivr_z (p0, z2, d0))

/*
** divr_1_f16_z_tied1:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	movprfx	z0\.h, p0/z, z0\.h
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f16_z_tied1, svfloat16_t,
		z0 = svdivr_n_f16_z (p0, z0, 1),
		z0 = svdivr_z (p0, z0, 1))

/*
** divr_0p5_f16_z_tied1:
**	fmov	(z[0-9]+\.h), #5.0(e\-1)?
**	movprfx	z0\.h, p0/z, z0\.h
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f16_z_tied1, svfloat16_t,
		z0 = svdivr_n_f16_z (p0, z0, 0.5),
		z0 = svdivr_z (p0, z0, 0.5))

/*
** divr_0p5_f16_z_untied:
**	fmov	(z[0-9]+\.h), #5.0(e\-1)?
**	movprfx	z0\.h, p0/z, \1
**	fdiv	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (divr_0p5_f16_z_untied, svfloat16_t,
		z0 = svdivr_n_f16_z (p0, z1, 0.5),
		z0 = svdivr_z (p0, z1, 0.5))

/*
** divr_f16_x_tied1:
**	fdivr	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_x_tied1, svfloat16_t,
		z0 = svdivr_f16_x (p0, z0, z1),
		z0 = svdivr_x (p0, z0, z1))

/*
** divr_f16_x_tied2:
**	fdiv	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_x_tied2, svfloat16_t,
		z1 = svdivr_f16_x (p0, z0, z1),
		z1 = svdivr_x (p0, z0, z1))

/*
** divr_f16_x_untied:
**	movprfx	z2, z1
**	fdiv	z2\.h, p0/m, z2\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (divr_f16_x_untied, svfloat16_t,
		z2 = svdivr_f16_x (p0, z0, z1),
		z2 = svdivr_x (p0, z0, z1))

/*
** divr_w0_f16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f16_x_tied1, svfloat16_t, __fp16,
		 z0 = svdivr_n_f16_x (p0, z0, x0),
		 z0 = svdivr_x (p0, z0, x0))

/*
** divr_w0_f16_x_untied:
**	mov	z1\.h, w0
**	fdiv	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (divr_w0_f16_x_untied, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_x (p0, z0, x0),
		 z1 = svdivr_x (p0, z0, x0))

/*
** divr_h0_f16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	fdivr	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (divr_h0_f16_x_tied1, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_x (p0, z1, d0),
		 z1 = svdivr_x (p0, z1, d0))

/*
** divr_h0_f16_x_untied:
**	mov	z2\.h, h0
**	fdiv	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (divr_h0_f16_x_untied, svfloat16_t, __fp16,
		 z2 = svdivr_n_f16_x (p0, z1, d0),
		 z2 = svdivr_x (p0, z1, d0))

/*
** divr_1_f16_x_tied1:
**	fmov	(z[0-9]+\.h), #1\.0(e\+0)?
**	fdivr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (divr_1_f16_x_tied1, svfloat16_t,
		z0 = svdivr_n_f16_x (p0, z0, 1),
		z0 = svdivr_x (p0, z0, 1))

/*
** divr_1_f16_x_untied:
**	fmov	z0\.h, #1\.0(e\+0)?
**	fdiv	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (divr_1_f16_x_untied, svfloat16_t,
		z0 = svdivr_n_f16_x (p0, z1, 1),
		z0 = svdivr_x (p0, z1, 1))

/*
** ptrue_divr_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fdivr	z0\.h, \1/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f16_x_tied1, svfloat16_t,
		z0 = svdivr_f16_x (svptrue_b16 (), z0, z1),
		z0 = svdivr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_divr_f16_x_tied2:
**	ptrue	(p[0-7])\.h[^\n]*
**	fdiv	z1\.h, \1/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f16_x_tied2, svfloat16_t,
		z1 = svdivr_f16_x (svptrue_b16 (), z0, z1),
		z1 = svdivr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_divr_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z2, z1
**	fdiv	z2\.h, \1/m, z2\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_f16_x_untied, svfloat16_t,
		z2 = svdivr_f16_x (svptrue_b16 (), z0, z1),
		z2 = svdivr_x (svptrue_b16 (), z0, z1))

/*
** ptrue_divr_w0_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), w0
**	fdivr	z0\.h, \1/m, z0\.h, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_w0_f16_x_tied1, svfloat16_t, __fp16,
		 z0 = svdivr_n_f16_x (svptrue_b16 (), z0, x0),
		 z0 = svdivr_x (svptrue_b16 (), z0, x0))

/*
** ptrue_divr_w0_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	z1\.h, w0
**	fdiv	z1\.h, \1/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_w0_f16_x_untied, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_x (svptrue_b16 (), z0, x0),
		 z1 = svdivr_x (svptrue_b16 (), z0, x0))

/*
** ptrue_divr_h0_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	(z[0-9]+\.h), h0
**	fdivr	z1\.h, \1/m, z1\.h, \2
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_h0_f16_x_tied1, svfloat16_t, __fp16,
		 z1 = svdivr_n_f16_x (svptrue_b16 (), z1, d0),
		 z1 = svdivr_x (svptrue_b16 (), z1, d0))

/*
** ptrue_divr_h0_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	mov	z2\.h, h0
**	fdiv	z2\.h, \1/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (ptrue_divr_h0_f16_x_untied, svfloat16_t, __fp16,
		 z2 = svdivr_n_f16_x (svptrue_b16 (), z1, d0),
		 z2 = svdivr_x (svptrue_b16 (), z1, d0))

/*
** ptrue_divr_1_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	fmov	z0\.h, #1.0(e\+0)?
**	fdiv	z0\.h, \1/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_divr_1_f16_x_untied, svfloat16_t,
		z0 = svdivr_n_f16_x (svptrue_b16 (), z1, 1),
		z0 = svdivr_x (svptrue_b16 (), z1, 1))

/*
** ptrue_b8_divr_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fdivr	z0\.h, \1/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f16_x_tied1, svfloat16_t,
		z0 = svdivr_f16_x (svptrue_b8 (), z0, z1),
		z0 = svdivr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_divr_f16_x_tied2:
**	ptrue	(p[0-7])\.h[^\n]*
**	fdiv	z1\.h, \1/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f16_x_tied2, svfloat16_t,
		z1 = svdivr_f16_x (svptrue_b8 (), z0, z1),
		z1 = svdivr_x (svptrue_b8 (), z0, z1))

/*
** ptrue_b8_divr_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z2, z1
**	fdiv	z2\.h, \1/m, z2\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_b8_divr_f16_x_untied, svfloat16_t,
		z2 = svdivr_f16_x (svptrue_b8 (), z0, z1),
		z2 = svdivr_x (svptrue_b8 (), z0, z1))
