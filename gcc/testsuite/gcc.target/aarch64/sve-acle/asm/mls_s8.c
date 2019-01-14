/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mls_s8_m_tied1:
**	mls	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_m_tied1, svint8_t,
		z0 = svmls_s8_m (p0, z0, z1, z2),
		z0 = svmls_m (p0, z0, z1, z2))

/*
** mls_s8_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.b, p0/m, \1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_m_tied2, svint8_t,
		z1 = svmls_s8_m (p0, z0, z1, z2),
		z1 = svmls_m (p0, z0, z1, z2))

/*
** mls_s8_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	mls	z2\.b, p0/m, z1\.b, \1\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_m_tied3, svint8_t,
		z2 = svmls_s8_m (p0, z0, z1, z2),
		z2 = svmls_m (p0, z0, z1, z2))

/*
** mls_s8_m_untied:
**	movprfx	z0, z1
**	mls	z0\.b, p0/m, z2\.b, z3\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_m_untied, svint8_t,
		z0 = svmls_s8_m (p0, z1, z2, z3),
		z0 = svmls_m (p0, z1, z2, z3))

/*
** mls_w0_s8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_m_tied1, svint8_t, int8_t,
		 z0 = svmls_n_s8_m (p0, z0, z1, x0),
		 z0 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_s8_m_tied2:
**	mov	(z[0-9]+\.b), w0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.b, p0/m, \2\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_m_tied2, svint8_t, int8_t,
		 z1 = svmls_n_s8_m (p0, z0, z1, x0),
		 z1 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_s8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	mls	z0\.b, p0/m, z2\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_m_untied, svint8_t, int8_t,
		 z0 = svmls_n_s8_m (p0, z1, z2, x0),
		 z0 = svmls_m (p0, z1, z2, x0))

/*
** mls_b0_s8_m_tied1:
**	mov	(z[0-9]+\.b), b0
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_m_tied1, svint8_t, int8_t,
		 z0 = svmls_n_s8_m (p0, z0, z1, d0),
		 z0 = svmls_m (p0, z0, z1, d0))

/*
** mls_b0_s8_m_tied2:
**	mov	(z[0-9]+\.b), b0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.b, p0/m, \2\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_m_tied2, svint8_t, int8_t,
		 z1 = svmls_n_s8_m (p0, z0, z1, d0),
		 z1 = svmls_m (p0, z0, z1, d0))

/*
** mls_b0_s8_m_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z0
**	mls	z1\.b, p0/m, z2\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_m_untied, svint8_t, int8_t,
		 z1 = svmls_n_s8_m (p0, z0, z2, d0),
		 z1 = svmls_m (p0, z0, z2, d0))

/*
** mls_2_s8_m_tied1:
**	mov	(z[0-9]+\.b), #2
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s8_m_tied1, svint8_t,
		z0 = svmls_n_s8_m (p0, z0, z1, 2),
		z0 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_s8_m_tied2:
**	mov	(z[0-9]+\.b), #2
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.b, p0/m, \2\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s8_m_tied2, svint8_t,
		z1 = svmls_n_s8_m (p0, z0, z1, 2),
		z1 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_s8_m_untied:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0, z1
**	mls	z0\.b, p0/m, z2\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s8_m_untied, svint8_t,
		z0 = svmls_n_s8_m (p0, z1, z2, 2),
		z0 = svmls_m (p0, z1, z2, 2))

/*
** mls_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	mls	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_z_tied1, svint8_t,
		z0 = svmls_s8_z (p0, z0, z1, z2),
		z0 = svmls_z (p0, z0, z1, z2))

/*
** mls_s8_z_tied2:
**	movprfx	z1\.b, p0/z, z1\.b
**	msb	z1\.b, p0/m, z2\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_z_tied2, svint8_t,
		z1 = svmls_s8_z (p0, z0, z1, z2),
		z1 = svmls_z (p0, z0, z1, z2))

/*
** mls_s8_z_tied3:
**	movprfx	z2\.b, p0/z, z2\.b
**	msb	z2\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_z_tied3, svint8_t,
		z2 = svmls_s8_z (p0, z0, z1, z2),
		z2 = svmls_z (p0, z0, z1, z2))

/*
** mls_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	mls	z0\.b, p0/m, z2\.b, z3\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_z_untied, svint8_t,
		z0 = svmls_s8_z (p0, z1, z2, z3),
		z0 = svmls_z (p0, z1, z2, z3))

/*
** mls_w0_s8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_z_tied1, svint8_t, int8_t,
		 z0 = svmls_n_s8_z (p0, z0, z1, x0),
		 z0 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_s8_z_tied2:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z1\.b, p0/z, z1\.b
**	msb	z1\.b, p0/m, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_z_tied2, svint8_t, int8_t,
		 z1 = svmls_n_s8_z (p0, z0, z1, x0),
		 z1 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_s8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z1\.b
**	mls	z0\.b, p0/m, z2\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_z_untied, svint8_t, int8_t,
		 z0 = svmls_n_s8_z (p0, z1, z2, x0),
		 z0 = svmls_z (p0, z1, z2, x0))

/*
** mls_b0_s8_z_tied1:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z0\.b, p0/z, z0\.b
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_z_tied1, svint8_t, int8_t,
		 z0 = svmls_n_s8_z (p0, z0, z1, d0),
		 z0 = svmls_z (p0, z0, z1, d0))

/*
** mls_b0_s8_z_tied2:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	msb	z1\.b, p0/m, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_z_tied2, svint8_t, int8_t,
		 z1 = svmls_n_s8_z (p0, z0, z1, d0),
		 z1 = svmls_z (p0, z0, z1, d0))

/*
** mls_b0_s8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z0\.b
**	mls	z1\.b, p0/m, z2\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_z_untied, svint8_t, int8_t,
		 z1 = svmls_n_s8_z (p0, z0, z2, d0),
		 z1 = svmls_z (p0, z0, z2, d0))

/*
** mls_s8_x_tied1:
**	mls	z0\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_x_tied1, svint8_t,
		z0 = svmls_s8_x (p0, z0, z1, z2),
		z0 = svmls_x (p0, z0, z1, z2))

/*
** mls_s8_x_tied2:
**	msb	z1\.b, p0/m, z2\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_x_tied2, svint8_t,
		z1 = svmls_s8_x (p0, z0, z1, z2),
		z1 = svmls_x (p0, z0, z1, z2))

/*
** mls_s8_x_tied3:
**	msb	z2\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_x_tied3, svint8_t,
		z2 = svmls_s8_x (p0, z0, z1, z2),
		z2 = svmls_x (p0, z0, z1, z2))

/*
** mls_s8_x_untied:
**	movprfx	z3, z0
**	mls	z3\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mls_s8_x_untied, svint8_t,
		z3 = svmls_s8_x (p0, z0, z1, z2),
		z3 = svmls_x (p0, z0, z1, z2))

/*
** mls_w0_s8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_x_tied1, svint8_t, int8_t,
		 z0 = svmls_n_s8_x (p0, z0, z1, x0),
		 z0 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_s8_x_tied2:
**	mov	(z[0-9]+\.b), w0
**	msb	z1\.b, p0/m, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_x_tied2, svint8_t, int8_t,
		 z1 = svmls_n_s8_x (p0, z0, z1, x0),
		 z1 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_s8_x_untied:
**	mov	z2\.b, w0
**	msb	z2\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s8_x_untied, svint8_t, int8_t,
		 z2 = svmls_n_s8_x (p0, z0, z1, x0),
		 z2 = svmls_x (p0, z0, z1, x0))

/*
** mls_b0_s8_x_tied1:
**	mov	(z[0-9]+\.b), b0
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_x_tied1, svint8_t, int8_t,
		 z0 = svmls_n_s8_x (p0, z0, z1, d0),
		 z0 = svmls_x (p0, z0, z1, d0))

/*
** mls_b0_s8_x_tied2:
**	mov	(z[0-9]+\.b), b0
**	msb	z1\.b, p0/m, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_x_tied2, svint8_t, int8_t,
		 z1 = svmls_n_s8_x (p0, z0, z1, d0),
		 z1 = svmls_x (p0, z0, z1, d0))

/*
** mls_b0_s8_x_untied:
**	mov	z2\.b, b0
**	msb	z2\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mls_b0_s8_x_untied, svint8_t, int8_t,
		 z2 = svmls_n_s8_x (p0, z0, z1, d0),
		 z2 = svmls_x (p0, z0, z1, d0))

/*
** mls_2_s8_x_tied1:
**	mov	(z[0-9]+\.b), #2
**	mls	z0\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s8_x_tied1, svint8_t,
		z0 = svmls_n_s8_x (p0, z0, z1, 2),
		z0 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_s8_x_tied2:
**	mov	(z[0-9]+\.b), #2
**	msb	z1\.b, p0/m, \1, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mls_2_s8_x_tied2, svint8_t,
		z1 = svmls_n_s8_x (p0, z0, z1, 2),
		z1 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_s8_x_untied:
**	mov	z0\.b, #2
**	msb	z0\.b, p0/m, z2\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mls_2_s8_x_untied, svint8_t,
		z0 = svmls_n_s8_x (p0, z1, z2, 2),
		z0 = svmls_x (p0, z1, z2, 2))

