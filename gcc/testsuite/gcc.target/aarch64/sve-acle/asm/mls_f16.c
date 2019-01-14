/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mls_f16_m_tied1:
**	fmls	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_m_tied1, svfloat16_t,
		z0 = svmls_f16_m (p0, z0, z1, z2),
		z0 = svmls_m (p0, z0, z1, z2))

/*
** mls_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmls	z1\.h, p0/m, \1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_m_tied2, svfloat16_t,
		z1 = svmls_f16_m (p0, z0, z1, z2),
		z1 = svmls_m (p0, z0, z1, z2))

/*
** mls_f16_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	fmls	z2\.h, p0/m, z1\.h, \1\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_m_tied3, svfloat16_t,
		z2 = svmls_f16_m (p0, z0, z1, z2),
		z2 = svmls_m (p0, z0, z1, z2))

/*
** mls_f16_m_untied:
**	movprfx	z0, z1
**	fmls	z0\.h, p0/m, z2\.h, z3\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_m_untied, svfloat16_t,
		z0 = svmls_f16_m (p0, z1, z2, z3),
		z0 = svmls_m (p0, z1, z2, z3))

/*
** mls_w0_f16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_m_tied1, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_m (p0, z0, z1, x0),
		 z0 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_f16_m_tied2:
**	mov	(z[0-9]+\.h), w0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmls	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_m_tied2, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_m (p0, z0, z1, x0),
		 z1 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_f16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	fmls	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_m_untied, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_m (p0, z1, z2, x0),
		 z0 = svmls_m (p0, z1, z2, x0))

/*
** mls_h0_f16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_m_tied1, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_m (p0, z0, z1, d0),
		 z0 = svmls_m (p0, z0, z1, d0))

/*
** mls_h0_f16_m_tied2:
**	mov	(z[0-9]+\.h), h0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmls	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_m_tied2, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_m (p0, z0, z1, d0),
		 z1 = svmls_m (p0, z0, z1, d0))

/*
** mls_h0_f16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z0
**	fmls	z1\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_m_untied, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_m (p0, z0, z2, d0),
		 z1 = svmls_m (p0, z0, z2, d0))

/*
** mls_2_f16_m_tied1:
**	fmov	(z[0-9]+\.h), #2.0(e\+0)?
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_f16_m_tied1, svfloat16_t,
		z0 = svmls_n_f16_m (p0, z0, z1, 2),
		z0 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_f16_m_tied2:
**	fmov	(z[0-9]+\.h), #2.0(e\+0)?
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	fmls	z1\.h, p0/m, \3\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_f16_m_tied2, svfloat16_t,
		z1 = svmls_n_f16_m (p0, z0, z1, 2),
		z1 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_f16_m_untied:
**	fmov	(z[0-9]+\.h), #2.0(e\+0)?
**	movprfx	z0, z1
**	fmls	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_f16_m_untied, svfloat16_t,
		z0 = svmls_n_f16_m (p0, z1, z2, 2),
		z0 = svmls_m (p0, z1, z2, 2))

/*
** mls_f16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	fmls	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_z_tied1, svfloat16_t,
		z0 = svmls_f16_z (p0, z0, z1, z2),
		z0 = svmls_z (p0, z0, z1, z2))

/*
** mls_f16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	fmsb	z1\.h, p0/m, z2\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_z_tied2, svfloat16_t,
		z1 = svmls_f16_z (p0, z0, z1, z2),
		z1 = svmls_z (p0, z0, z1, z2))

/*
** mls_f16_z_tied3:
**	movprfx	z2\.h, p0/z, z2\.h
**	fmsb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_z_tied3, svfloat16_t,
		z2 = svmls_f16_z (p0, z0, z1, z2),
		z2 = svmls_z (p0, z0, z1, z2))

/*
** mls_f16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	fmls	z0\.h, p0/m, z2\.h, z3\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_z_untied, svfloat16_t,
		z0 = svmls_f16_z (p0, z1, z2, z3),
		z0 = svmls_z (p0, z1, z2, z3))

/*
** mls_w0_f16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_z_tied1, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_z (p0, z0, z1, x0),
		 z0 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_f16_z_tied2:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z1\.h, p0/z, z1\.h
**	fmsb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_z_tied2, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_z (p0, z0, z1, x0),
		 z1 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_f16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z1\.h
**	fmls	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_z_untied, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_z (p0, z1, z2, x0),
		 z0 = svmls_z (p0, z1, z2, x0))

/*
** mls_h0_f16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z0\.h, p0/z, z0\.h
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_z_tied1, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_z (p0, z0, z1, d0),
		 z0 = svmls_z (p0, z0, z1, d0))

/*
** mls_h0_f16_z_tied2:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	fmsb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_z_tied2, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_z (p0, z0, z1, d0),
		 z1 = svmls_z (p0, z0, z1, d0))

/*
** mls_h0_f16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z0\.h
**	fmls	z1\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_z_untied, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_z (p0, z0, z2, d0),
		 z1 = svmls_z (p0, z0, z2, d0))

/*
** mls_f16_x_tied1:
**	fmls	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_x_tied1, svfloat16_t,
		z0 = svmls_f16_x (p0, z0, z1, z2),
		z0 = svmls_x (p0, z0, z1, z2))

/*
** mls_f16_x_tied2:
**	fmsb	z1\.h, p0/m, z2\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_x_tied2, svfloat16_t,
		z1 = svmls_f16_x (p0, z0, z1, z2),
		z1 = svmls_x (p0, z0, z1, z2))

/*
** mls_f16_x_tied3:
**	fmsb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_x_tied3, svfloat16_t,
		z2 = svmls_f16_x (p0, z0, z1, z2),
		z2 = svmls_x (p0, z0, z1, z2))

/*
** mls_f16_x_untied:
**	movprfx	z3, z0
**	fmls	z3\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_f16_x_untied, svfloat16_t,
		z3 = svmls_f16_x (p0, z0, z1, z2),
		z3 = svmls_x (p0, z0, z1, z2))

/*
** mls_w0_f16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_x_tied1, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_x (p0, z0, z1, x0),
		 z0 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_f16_x_tied2:
**	mov	(z[0-9]+\.h), w0
**	fmsb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_x_tied2, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_x (p0, z0, z1, x0),
		 z1 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_f16_x_untied:
**	mov	z2\.h, w0
**	fmsb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_f16_x_untied, svfloat16_t, __fp16,
		 z2 = svmls_n_f16_x (p0, z0, z1, x0),
		 z2 = svmls_x (p0, z0, z1, x0))

/*
** mls_h0_f16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_x_tied1, svfloat16_t, __fp16,
		 z0 = svmls_n_f16_x (p0, z0, z1, d0),
		 z0 = svmls_x (p0, z0, z1, d0))

/*
** mls_h0_f16_x_tied2:
**	mov	(z[0-9]+\.h), h0
**	fmsb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_x_tied2, svfloat16_t, __fp16,
		 z1 = svmls_n_f16_x (p0, z0, z1, d0),
		 z1 = svmls_x (p0, z0, z1, d0))

/*
** mls_h0_f16_x_untied:
**	mov	z2\.h, h0
**	fmsb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_f16_x_untied, svfloat16_t, __fp16,
		 z2 = svmls_n_f16_x (p0, z0, z1, d0),
		 z2 = svmls_x (p0, z0, z1, d0))

/*
** mls_2_f16_x_tied1:
**	fmov	(z[0-9]+\.h), #2.0(e\+0)?
**	fmls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_f16_x_tied1, svfloat16_t,
		z0 = svmls_n_f16_x (p0, z0, z1, 2),
		z0 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_f16_x_tied2:
**	fmov	(z[0-9]+\.h), #2.0(e\+0)?
**	fmsb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_2_f16_x_tied2, svfloat16_t,
		z1 = svmls_n_f16_x (p0, z0, z1, 2),
		z1 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_f16_x_untied:
**	fmov	z0\.h, #2.0(e\+0)?
**	fmsb	z0\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mls_2_f16_x_untied, svfloat16_t,
		z0 = svmls_n_f16_x (p0, z1, z2, 2),
		z0 = svmls_x (p0, z1, z2, 2))

