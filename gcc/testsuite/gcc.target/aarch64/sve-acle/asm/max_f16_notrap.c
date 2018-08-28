/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */
/* { dg-additional-options "-fno-trapping-math" } */

#include "test_sve_acle.h"

/*
** max_f16_m_tied1:
**	fmax	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (max_f16_m_tied1, svfloat16_t,
		z0 = svmax_f16_m (p0, z0, z1),
		z0 = svmax_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (max_f16_m_tied2, svfloat16_t,
		z1 = svmax_f16_m (p0, z0, z1),
		z1 = svmax_m (p0, z0, z1))

/*
** max_f16_m_untied:
**	movprfx	z0, z1
**	fmax	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (max_f16_m_untied, svfloat16_t,
		z0 = svmax_f16_m (p0, z1, z2),
		z0 = svmax_m (p0, z1, z2))

/*
** max_w0_f16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_w0_f16_m_tied1, svfloat16_t, __fp16,
		 z0 = svmax_n_f16_m (p0, z0, x0),
		 z0 = svmax_m (p0, z0, x0))

/*
** max_w0_f16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_w0_f16_m_untied, svfloat16_t, __fp16,
		 z0 = svmax_n_f16_m (p0, z1, x0),
		 z0 = svmax_m (p0, z1, x0))

/*
** max_h0_f16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	fmax	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_h0_f16_m_tied1, svfloat16_t, __fp16,
		 z1 = svmax_n_f16_m (p0, z1, d0),
		 z1 = svmax_m (p0, z1, d0))

/*
** max_h0_f16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z2
**	fmax	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_h0_f16_m_untied, svfloat16_t, __fp16,
		 z1 = svmax_n_f16_m (p0, z2, d0),
		 z1 = svmax_m (p0, z2, d0))

/*
** max_1_f16_m_tied1:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f16_m_tied1, svfloat16_t,
		z0 = svmax_n_f16_m (p0, z0, 1),
		z0 = svmax_m (p0, z0, 1))

/*
** max_1_f16_m_untied:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	movprfx	z0, z1
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f16_m_untied, svfloat16_t,
		z0 = svmax_n_f16_m (p0, z1, 1),
		z0 = svmax_m (p0, z1, 1))

/*
** max_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fmax	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (max_f16_z_tied1, svfloat16_t,
		z0 = svmax_f16_z (p0, z0, z1),
		z0 = svmax_z (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (max_f16_z_tied2, svfloat16_t,
		z1 = svmax_f16_z (p0, z0, z1),
		z1 = svmax_z (p0, z0, z1))

/*
** max_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fmax	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (max_f16_z_untied, svfloat16_t,
		z0 = svmax_f16_z (p0, z1, z2),
		z0 = svmax_z (p0, z1, z2))

/*
** max_w0_f16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_w0_f16_z_tied1, svfloat16_t, __fp16,
		 z0 = svmax_n_f16_z (p0, z0, x0),
		 z0 = svmax_z (p0, z0, x0))

/*
** max_w0_f16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z1\.h
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_w0_f16_z_untied, svfloat16_t, __fp16,
		 z0 = svmax_n_f16_z (p0, z1, x0),
		 z0 = svmax_z (p0, z1, x0))

/*
** max_h0_f16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	fmax	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_h0_f16_z_tied1, svfloat16_t, __fp16,
		 z1 = svmax_n_f16_z (p0, z1, d0),
		 z1 = svmax_z (p0, z1, d0))

/*
** max_h0_f16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z2\.h
**	fmax	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_h0_f16_z_untied, svfloat16_t, __fp16,
		 z1 = svmax_n_f16_z (p0, z2, d0),
		 z1 = svmax_z (p0, z2, d0))

/*
** max_1_f16_z_untied:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	movprfx	z0\.h, p0/z, z1\.h
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f16_z_untied, svfloat16_t,
		z0 = svmax_n_f16_z (p0, z1, 1),
		z0 = svmax_z (p0, z1, 1))

/*
** max_1_f16_z_tied1:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	movprfx	z0\.h, p0/z, z0\.h
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f16_z_tied1, svfloat16_t,
		z0 = svmax_n_f16_z (p0, z0, 1),
		z0 = svmax_z (p0, z0, 1))

/*
** max_f16_x_tied1:
**	fmax	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (max_f16_x_tied1, svfloat16_t,
		z0 = svmax_f16_x (p0, z0, z1),
		z0 = svmax_x (p0, z0, z1))

/*
** max_f16_x_tied2:
**	fmax	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (max_f16_x_tied2, svfloat16_t,
		z1 = svmax_f16_x (p0, z0, z1),
		z1 = svmax_x (p0, z0, z1))

/*
** max_f16_x_untied:
**	movprfx	z2, z0
**	fmax	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (max_f16_x_untied, svfloat16_t,
		z2 = svmax_f16_x (p0, z0, z1),
		z2 = svmax_x (p0, z0, z1))

/*
** max_w0_f16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_w0_f16_x_tied1, svfloat16_t, __fp16,
		 z0 = svmax_n_f16_x (p0, z0, x0),
		 z0 = svmax_x (p0, z0, x0))

/*
** max_w0_f16_x_untied:
**	mov	z1\.h, w0
**	fmax	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (max_w0_f16_x_untied, svfloat16_t, __fp16,
		 z1 = svmax_n_f16_x (p0, z0, x0),
		 z1 = svmax_x (p0, z0, x0))

/*
** max_h0_f16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	fmax	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (max_h0_f16_x_tied1, svfloat16_t, __fp16,
		 z1 = svmax_n_f16_x (p0, z1, d0),
		 z1 = svmax_x (p0, z1, d0))

/*
** max_h0_f16_x_untied:
**	mov	z2\.h, h0
**	fmax	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (max_h0_f16_x_untied, svfloat16_t, __fp16,
		 z2 = svmax_n_f16_x (p0, z1, d0),
		 z2 = svmax_x (p0, z1, d0))

/*
** max_1_f16_x_tied1:
**	fmov	(z[0-9]+\.h), #1.0(e\+0)?
**	fmax	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_f16_x_tied1, svfloat16_t,
		z0 = svmax_n_f16_x (p0, z0, 1),
		z0 = svmax_x (p0, z0, 1))

/*
** max_1_f16_x_untied:
**	fmov	z0\.h, #1.0(e\+0)?
**	fmax	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (max_1_f16_x_untied, svfloat16_t,
		z0 = svmax_n_f16_x (p0, z1, 1),
		z0 = svmax_x (p0, z1, 1))

/*
** ptrue_max_f16_x_tied1:
**	ptrue	(p[0-7])\.h[^\n]*
**	fmax	z0\.h, \1/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_max_f16_x_tied1, svfloat16_t,
		z0 = svmax_f16_x (svptrue_b16 (), z0, z1),
		z0 = svmax_x (svptrue_b16 (), z0, z1))

/*
** ptrue_max_f16_x_tied2:
**	ptrue	(p[0-7])\.h[^\n]*
**	fmax	z1\.h, \1/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_max_f16_x_tied2, svfloat16_t,
		z1 = svmax_f16_x (svptrue_b16 (), z0, z1),
		z1 = svmax_x (svptrue_b16 (), z0, z1))

/*
** ptrue_max_f16_x_untied:
**	ptrue	(p[0-7])\.h[^\n]*
**	movprfx	z2, z0
**	fmax	z2\.h, \1/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (ptrue_max_f16_x_untied, svfloat16_t,
		z2 = svmax_f16_x (svptrue_b16 (), z0, z1),
		z2 = svmax_x (svptrue_b16 (), z0, z1))
