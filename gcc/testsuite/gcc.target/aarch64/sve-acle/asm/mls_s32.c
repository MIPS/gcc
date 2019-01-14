/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mls_s32_m_tied1:
**	mls	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_tied1, svint32_t,
		z0 = svmls_s32_m (p0, z0, z1, z2),
		z0 = svmls_m (p0, z0, z1, z2))

/*
** mls_s32_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.s, p0/m, \1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_tied2, svint32_t,
		z1 = svmls_s32_m (p0, z0, z1, z2),
		z1 = svmls_m (p0, z0, z1, z2))

/*
** mls_s32_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	mls	z2\.s, p0/m, z1\.s, \1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_tied3, svint32_t,
		z2 = svmls_s32_m (p0, z0, z1, z2),
		z2 = svmls_m (p0, z0, z1, z2))

/*
** mls_s32_m_untied:
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, z3\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_m_untied, svint32_t,
		z0 = svmls_s32_m (p0, z1, z2, z3),
		z0 = svmls_m (p0, z1, z2, z3))

/*
** mls_w0_s32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_m_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_m (p0, z0, z1, x0),
		 z0 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_s32_m_tied2:
**	mov	(z[0-9]+\.s), w0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.s, p0/m, \2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_m_tied2, svint32_t, int32_t,
		 z1 = svmls_n_s32_m (p0, z0, z1, x0),
		 z1 = svmls_m (p0, z0, z1, x0))

/*
** mls_w0_s32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_m_untied, svint32_t, int32_t,
		 z0 = svmls_n_s32_m (p0, z1, z2, x0),
		 z0 = svmls_m (p0, z1, z2, x0))

/*
** mls_s0_s32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_m_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_m (p0, z0, z1, d0),
		 z0 = svmls_m (p0, z0, z1, d0))

/*
** mls_s0_s32_m_tied2:
**	mov	(z[0-9]+\.s), s0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.s, p0/m, \2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_m_tied2, svint32_t, int32_t,
		 z1 = svmls_n_s32_m (p0, z0, z1, d0),
		 z1 = svmls_m (p0, z0, z1, d0))

/*
** mls_s0_s32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z0
**	mls	z1\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_m_untied, svint32_t, int32_t,
		 z1 = svmls_n_s32_m (p0, z0, z2, d0),
		 z1 = svmls_m (p0, z0, z2, d0))

/*
** mls_2_s32_m_tied1:
**	mov	(z[0-9]+\.s), #2
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s32_m_tied1, svint32_t,
		z0 = svmls_n_s32_m (p0, z0, z1, 2),
		z0 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_s32_m_tied2:
**	mov	(z[0-9]+\.s), #2
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mls	z1\.s, p0/m, \2\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s32_m_tied2, svint32_t,
		z1 = svmls_n_s32_m (p0, z0, z1, 2),
		z1 = svmls_m (p0, z0, z1, 2))

/*
** mls_2_s32_m_untied:
**	mov	(z[0-9]+\.s), #2
**	movprfx	z0, z1
**	mls	z0\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s32_m_untied, svint32_t,
		z0 = svmls_n_s32_m (p0, z1, z2, 2),
		z0 = svmls_m (p0, z1, z2, 2))

/*
** mls_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	mls	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_tied1, svint32_t,
		z0 = svmls_s32_z (p0, z0, z1, z2),
		z0 = svmls_z (p0, z0, z1, z2))

/*
** mls_s32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	msb	z1\.s, p0/m, z2\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_tied2, svint32_t,
		z1 = svmls_s32_z (p0, z0, z1, z2),
		z1 = svmls_z (p0, z0, z1, z2))

/*
** mls_s32_z_tied3:
**	movprfx	z2\.s, p0/z, z2\.s
**	msb	z2\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_tied3, svint32_t,
		z2 = svmls_s32_z (p0, z0, z1, z2),
		z2 = svmls_z (p0, z0, z1, z2))

/*
** mls_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	mls	z0\.s, p0/m, z2\.s, z3\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_z_untied, svint32_t,
		z0 = svmls_s32_z (p0, z1, z2, z3),
		z0 = svmls_z (p0, z1, z2, z3))

/*
** mls_w0_s32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_z_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_z (p0, z0, z1, x0),
		 z0 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_s32_z_tied2:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z1\.s, p0/z, z1\.s
**	msb	z1\.s, p0/m, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_z_tied2, svint32_t, int32_t,
		 z1 = svmls_n_s32_z (p0, z0, z1, x0),
		 z1 = svmls_z (p0, z0, z1, x0))

/*
** mls_w0_s32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z1\.s
**	mls	z0\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_z_untied, svint32_t, int32_t,
		 z0 = svmls_n_s32_z (p0, z1, z2, x0),
		 z0 = svmls_z (p0, z1, z2, x0))

/*
** mls_s0_s32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z0\.s, p0/z, z0\.s
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_z_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_z (p0, z0, z1, d0),
		 z0 = svmls_z (p0, z0, z1, d0))

/*
** mls_s0_s32_z_tied2:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	msb	z1\.s, p0/m, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_z_tied2, svint32_t, int32_t,
		 z1 = svmls_n_s32_z (p0, z0, z1, d0),
		 z1 = svmls_z (p0, z0, z1, d0))

/*
** mls_s0_s32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z0\.s
**	mls	z1\.s, p0/m, z2\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_z_untied, svint32_t, int32_t,
		 z1 = svmls_n_s32_z (p0, z0, z2, d0),
		 z1 = svmls_z (p0, z0, z2, d0))

/*
** mls_s32_x_tied1:
**	mls	z0\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_tied1, svint32_t,
		z0 = svmls_s32_x (p0, z0, z1, z2),
		z0 = svmls_x (p0, z0, z1, z2))

/*
** mls_s32_x_tied2:
**	msb	z1\.s, p0/m, z2\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_tied2, svint32_t,
		z1 = svmls_s32_x (p0, z0, z1, z2),
		z1 = svmls_x (p0, z0, z1, z2))

/*
** mls_s32_x_tied3:
**	msb	z2\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_tied3, svint32_t,
		z2 = svmls_s32_x (p0, z0, z1, z2),
		z2 = svmls_x (p0, z0, z1, z2))

/*
** mls_s32_x_untied:
**	movprfx	z3, z0
**	mls	z3\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mls_s32_x_untied, svint32_t,
		z3 = svmls_s32_x (p0, z0, z1, z2),
		z3 = svmls_x (p0, z0, z1, z2))

/*
** mls_w0_s32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_x_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_x (p0, z0, z1, x0),
		 z0 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_s32_x_tied2:
**	mov	(z[0-9]+\.s), w0
**	msb	z1\.s, p0/m, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_x_tied2, svint32_t, int32_t,
		 z1 = svmls_n_s32_x (p0, z0, z1, x0),
		 z1 = svmls_x (p0, z0, z1, x0))

/*
** mls_w0_s32_x_untied:
**	mov	z2\.s, w0
**	msb	z2\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mls_w0_s32_x_untied, svint32_t, int32_t,
		 z2 = svmls_n_s32_x (p0, z0, z1, x0),
		 z2 = svmls_x (p0, z0, z1, x0))

/*
** mls_s0_s32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_x_tied1, svint32_t, int32_t,
		 z0 = svmls_n_s32_x (p0, z0, z1, d0),
		 z0 = svmls_x (p0, z0, z1, d0))

/*
** mls_s0_s32_x_tied2:
**	mov	(z[0-9]+\.s), s0
**	msb	z1\.s, p0/m, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_x_tied2, svint32_t, int32_t,
		 z1 = svmls_n_s32_x (p0, z0, z1, d0),
		 z1 = svmls_x (p0, z0, z1, d0))

/*
** mls_s0_s32_x_untied:
**	mov	z2\.s, s0
**	msb	z2\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mls_s0_s32_x_untied, svint32_t, int32_t,
		 z2 = svmls_n_s32_x (p0, z0, z1, d0),
		 z2 = svmls_x (p0, z0, z1, d0))

/*
** mls_2_s32_x_tied1:
**	mov	(z[0-9]+\.s), #2
**	mls	z0\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mls_2_s32_x_tied1, svint32_t,
		z0 = svmls_n_s32_x (p0, z0, z1, 2),
		z0 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_s32_x_tied2:
**	mov	(z[0-9]+\.s), #2
**	msb	z1\.s, p0/m, \1, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mls_2_s32_x_tied2, svint32_t,
		z1 = svmls_n_s32_x (p0, z0, z1, 2),
		z1 = svmls_x (p0, z0, z1, 2))

/*
** mls_2_s32_x_untied:
**	mov	z0\.s, #2
**	msb	z0\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mls_2_s32_x_untied, svint32_t,
		z0 = svmls_n_s32_x (p0, z1, z2, 2),
		z0 = svmls_x (p0, z1, z2, 2))

