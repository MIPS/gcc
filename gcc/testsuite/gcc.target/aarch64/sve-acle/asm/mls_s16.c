/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mls_s16_m_tied1:
**	mls	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_m_tied1, svint16_t,
		z0 = svmls_s16_m (p0, z0, z1, z2),
		z0 = svmls_m (p0, z0, z1, z2))

/*
** mls_s16_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.h, p0/m, \1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_m_tied2, svint16_t,
		z1 = svmls_s16_m (p0, z0, z1, z2),
		z1 = svmls_m (p0, z0, z1, z2))

/*
** mls_s16_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	mls	z2\.h, p0/m, z1\.h, \1\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_m_tied3, svint16_t,
		z2 = svmls_s16_m (p0, z0, z1, z2),
		z2 = svmls_m (p0, z0, z1, z2))

/*
** mls_s16_m_untied:
**	movprfx	z0, z1
**	mls	z0\.h, p0/m, z2\.h, z3\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_m_untied, svint16_t,
		z0 = svmls_s16_m (p0, z1, z2, z3),
		z0 = svmls_m (p0, z1, z2, z3))

/*
** mls_w0_s16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_m_tied1, svint16_t, int16_t,
		 z0 = svmls_n_s16_m (p0, z0, z1, x0),
		 z0 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_s16_m_tied2:
**	mov	(z[0-9]+\.h), w0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_m_tied2, svint16_t, int16_t,
		 z1 = svmls_n_s16_m (p0, z0, z1, x0),
		 z1 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_s16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	mls	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_m_untied, svint16_t, int16_t,
		 z0 = svmls_n_s16_m (p0, z1, z2, x0),
		 z0 = svmls_m (p0, z1, z2, x0))

/*
** mls_h0_s16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_m_tied1, svint16_t, int16_t,
		 z0 = svmls_n_s16_m (p0, z0, z1, d0),
		 z0 = svmls_m (p0, z0, z1, d0))

/*
** mls_h0_s16_m_tied2:
**	mov	(z[0-9]+\.h), h0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_m_tied2, svint16_t, int16_t,
		 z1 = svmls_n_s16_m (p0, z0, z1, d0),
		 z1 = svmls_m (p0, z0, z1, d0))

/*
** mls_h0_s16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z0
**	mls	z1\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_m_untied, svint16_t, int16_t,
		 z1 = svmls_n_s16_m (p0, z0, z2, d0),
		 z1 = svmls_m (p0, z0, z2, d0))

/*
** mls_2_s16_m_tied1:
**	mov	(z[0-9]+\.h), #2
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s16_m_tied1, svint16_t,
		z0 = svmls_n_s16_m (p0, z0, z1, 2),
		z0 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_s16_m_tied2:
**	mov	(z[0-9]+\.h), #2
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s16_m_tied2, svint16_t,
		z1 = svmls_n_s16_m (p0, z0, z1, 2),
		z1 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_s16_m_untied:
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0, z1
**	mls	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s16_m_untied, svint16_t,
		z0 = svmls_n_s16_m (p0, z1, z2, 2),
		z0 = svmls_m (p0, z1, z2, 2))

/*
** mls_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	mls	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_z_tied1, svint16_t,
		z0 = svmls_s16_z (p0, z0, z1, z2),
		z0 = svmls_z (p0, z0, z1, z2))

/*
** mls_s16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	msb	z1\.h, p0/m, z2\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_z_tied2, svint16_t,
		z1 = svmls_s16_z (p0, z0, z1, z2),
		z1 = svmls_z (p0, z0, z1, z2))

/*
** mls_s16_z_tied3:
**	movprfx	z2\.h, p0/z, z2\.h
**	msb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_z_tied3, svint16_t,
		z2 = svmls_s16_z (p0, z0, z1, z2),
		z2 = svmls_z (p0, z0, z1, z2))

/*
** mls_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	mls	z0\.h, p0/m, z2\.h, z3\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_z_untied, svint16_t,
		z0 = svmls_s16_z (p0, z1, z2, z3),
		z0 = svmls_z (p0, z1, z2, z3))

/*
** mls_w0_s16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_z_tied1, svint16_t, int16_t,
		 z0 = svmls_n_s16_z (p0, z0, z1, x0),
		 z0 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_s16_z_tied2:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z1\.h, p0/z, z1\.h
**	msb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_z_tied2, svint16_t, int16_t,
		 z1 = svmls_n_s16_z (p0, z0, z1, x0),
		 z1 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_s16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z1\.h
**	mls	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_z_untied, svint16_t, int16_t,
		 z0 = svmls_n_s16_z (p0, z1, z2, x0),
		 z0 = svmls_z (p0, z1, z2, x0))

/*
** mls_h0_s16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z0\.h, p0/z, z0\.h
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_z_tied1, svint16_t, int16_t,
		 z0 = svmls_n_s16_z (p0, z0, z1, d0),
		 z0 = svmls_z (p0, z0, z1, d0))

/*
** mls_h0_s16_z_tied2:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	msb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_z_tied2, svint16_t, int16_t,
		 z1 = svmls_n_s16_z (p0, z0, z1, d0),
		 z1 = svmls_z (p0, z0, z1, d0))

/*
** mls_h0_s16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z0\.h
**	mls	z1\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_z_untied, svint16_t, int16_t,
		 z1 = svmls_n_s16_z (p0, z0, z2, d0),
		 z1 = svmls_z (p0, z0, z2, d0))

/*
** mls_s16_x_tied1:
**	mls	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_x_tied1, svint16_t,
		z0 = svmls_s16_x (p0, z0, z1, z2),
		z0 = svmls_x (p0, z0, z1, z2))

/*
** mls_s16_x_tied2:
**	msb	z1\.h, p0/m, z2\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_x_tied2, svint16_t,
		z1 = svmls_s16_x (p0, z0, z1, z2),
		z1 = svmls_x (p0, z0, z1, z2))

/*
** mls_s16_x_tied3:
**	msb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_x_tied3, svint16_t,
		z2 = svmls_s16_x (p0, z0, z1, z2),
		z2 = svmls_x (p0, z0, z1, z2))

/*
** mls_s16_x_untied:
**	movprfx	z3, z0
**	mls	z3\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mls_s16_x_untied, svint16_t,
		z3 = svmls_s16_x (p0, z0, z1, z2),
		z3 = svmls_x (p0, z0, z1, z2))

/*
** mls_w0_s16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_x_tied1, svint16_t, int16_t,
		 z0 = svmls_n_s16_x (p0, z0, z1, x0),
		 z0 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_s16_x_tied2:
**	mov	(z[0-9]+\.h), w0
**	msb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_x_tied2, svint16_t, int16_t,
		 z1 = svmls_n_s16_x (p0, z0, z1, x0),
		 z1 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_s16_x_untied:
**	mov	z2\.h, w0
**	msb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s16_x_untied, svint16_t, int16_t,
		 z2 = svmls_n_s16_x (p0, z0, z1, x0),
		 z2 = svmls_x (p0, z0, z1, x0))

/*
** mls_h0_s16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_x_tied1, svint16_t, int16_t,
		 z0 = svmls_n_s16_x (p0, z0, z1, d0),
		 z0 = svmls_x (p0, z0, z1, d0))

/*
** mls_h0_s16_x_tied2:
**	mov	(z[0-9]+\.h), h0
**	msb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_x_tied2, svint16_t, int16_t,
		 z1 = svmls_n_s16_x (p0, z0, z1, d0),
		 z1 = svmls_x (p0, z0, z1, d0))

/*
** mls_h0_s16_x_untied:
**	mov	z2\.h, h0
**	msb	z2\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mls_h0_s16_x_untied, svint16_t, int16_t,
		 z2 = svmls_n_s16_x (p0, z0, z1, d0),
		 z2 = svmls_x (p0, z0, z1, d0))

/*
** mls_2_s16_x_tied1:
**	mov	(z[0-9]+\.h), #2
**	mls	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s16_x_tied1, svint16_t,
		z0 = svmls_n_s16_x (p0, z0, z1, 2),
		z0 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_s16_x_tied2:
**	mov	(z[0-9]+\.h), #2
**	msb	z1\.h, p0/m, \1, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mls_2_s16_x_tied2, svint16_t,
		z1 = svmls_n_s16_x (p0, z0, z1, 2),
		z1 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_s16_x_untied:
**	mov	z0\.h, #2
**	msb	z0\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mls_2_s16_x_untied, svint16_t,
		z0 = svmls_n_s16_x (p0, z1, z2, 2),
		z0 = svmls_x (p0, z1, z2, 2))

