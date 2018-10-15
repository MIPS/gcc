/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mul_s64_m_tied1:
**	mul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_m_tied1, svint64_t,
		z0 = svmul_s64_m (p0, z0, z1),
		z0 = svmul_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mul_s64_m_tied2, svint64_t,
		z1 = svmul_s64_m (p0, z0, z1),
		z1 = svmul_m (p0, z0, z1))

/*
** mul_s64_m_untied:
**	movprfx	z0, z1
**	mul	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_m_untied, svint64_t,
		z0 = svmul_s64_m (p0, z1, z2),
		z0 = svmul_m (p0, z1, z2))

/*
** mul_x0_s64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_s64_m_tied1, svint64_t, int64_t,
		 z0 = svmul_n_s64_m (p0, z0, x0),
		 z0 = svmul_m (p0, z0, x0))

/*
** mul_x0_s64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_s64_m_untied, svint64_t, int64_t,
		 z0 = svmul_n_s64_m (p0, z1, x0),
		 z0 = svmul_m (p0, z1, x0))

/*
** mul_d0_s64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	mul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_s64_m_tied1, svint64_t, int64_t,
		 z1 = svmul_n_s64_m (p0, z1, d0),
		 z1 = svmul_m (p0, z1, d0))

/*
** mul_d0_s64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	mul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_s64_m_untied, svint64_t, int64_t,
		 z1 = svmul_n_s64_m (p0, z2, d0),
		 z1 = svmul_m (p0, z2, d0))

/*
** mul_2_s64_m_tied1:
**	mov	(z[0-9]+\.d), #2
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_s64_m_tied1, svint64_t,
		z0 = svmul_n_s64_m (p0, z0, 2),
		z0 = svmul_m (p0, z0, 2))

/*
** mul_2_s64_m_untied:
**	mov	(z[0-9]+\.d), #2
**	movprfx	z0, z1
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_s64_m_untied, svint64_t,
		z0 = svmul_n_s64_m (p0, z1, 2),
		z0 = svmul_m (p0, z1, 2))

/*
** mul_m1_s64_m:
**	mov	(z[0-9]+)\.b, #(-1|255)
**	mul	z0\.d, p0/m, z0\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_m1_s64_m, svint64_t,
		z0 = svmul_n_s64_m (p0, z0, -1),
		z0 = svmul_m (p0, z0, -1))

/*
** mul_s64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	mul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_z_tied1, svint64_t,
		z0 = svmul_s64_z (p0, z0, z1),
		z0 = svmul_z (p0, z0, z1))

/*
** mul_s64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	mul	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_z_tied2, svint64_t,
		z1 = svmul_s64_z (p0, z0, z1),
		z1 = svmul_z (p0, z0, z1))

/*
** mul_s64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	mul	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_z_untied, svint64_t,
		z0 = svmul_s64_z (p0, z1, z2),
		z0 = svmul_z (p0, z1, z2))

/*
** mul_x0_s64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_s64_z_tied1, svint64_t, int64_t,
		 z0 = svmul_n_s64_z (p0, z0, x0),
		 z0 = svmul_z (p0, z0, x0))

/*
** mul_x0_s64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_s64_z_untied, svint64_t, int64_t,
		 z0 = svmul_n_s64_z (p0, z1, x0),
		 z0 = svmul_z (p0, z1, x0))

/*
** mul_d0_s64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	mul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_s64_z_tied1, svint64_t, int64_t,
		 z1 = svmul_n_s64_z (p0, z1, d0),
		 z1 = svmul_z (p0, z1, d0))

/*
** mul_d0_s64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z2\.d
**	mul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_s64_z_untied, svint64_t, int64_t,
		 z1 = svmul_n_s64_z (p0, z2, d0),
		 z1 = svmul_z (p0, z2, d0))

/*
** mul_s64_x_tied1:
**	mul	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_x_tied1, svint64_t,
		z0 = svmul_s64_x (p0, z0, z1),
		z0 = svmul_x (p0, z0, z1))

/*
** mul_s64_x_tied2:
**	mul	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_x_tied2, svint64_t,
		z1 = svmul_s64_x (p0, z0, z1),
		z1 = svmul_x (p0, z0, z1))

/*
** mul_s64_x_untied:
**	movprfx	z2, z0
**	mul	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mul_s64_x_untied, svint64_t,
		z2 = svmul_s64_x (p0, z0, z1),
		z2 = svmul_x (p0, z0, z1))

/*
** mul_x0_s64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_s64_x_tied1, svint64_t, int64_t,
		 z0 = svmul_n_s64_x (p0, z0, x0),
		 z0 = svmul_x (p0, z0, x0))

/*
** mul_x0_s64_x_untied:
**	mov	z1\.d, x0
**	mul	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mul_x0_s64_x_untied, svint64_t, int64_t,
		 z1 = svmul_n_s64_x (p0, z0, x0),
		 z1 = svmul_x (p0, z0, x0))

/*
** mul_d0_s64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	mul	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_s64_x_tied1, svint64_t, int64_t,
		 z1 = svmul_n_s64_x (p0, z1, d0),
		 z1 = svmul_x (p0, z1, d0))

/*
** mul_d0_s64_x_untied:
**	mov	z2\.d, d0
**	mul	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (mul_d0_s64_x_untied, svint64_t, int64_t,
		 z2 = svmul_n_s64_x (p0, z1, d0),
		 z2 = svmul_x (p0, z1, d0))

/*
** mul_2_s64_x_tied1:
**	mul	z0\.d, z0\.d, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_s64_x_tied1, svint64_t,
		z0 = svmul_n_s64_x (p0, z0, 2),
		z0 = svmul_x (p0, z0, 2))

/*
** mul_2_s64_x_untied:
**	mov	z0\.d, z1\.d
**	mul	z0\.d, z0\.d, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_s64_x_untied, svint64_t,
		z0 = svmul_n_s64_x (p0, z1, 2),
		z0 = svmul_x (p0, z1, 2))

/*
** mul_127_s64_x:
**	mul	z0\.d, z0\.d, #127
**	ret
*/
TEST_UNIFORM_Z (mul_127_s64_x, svint64_t,
		z0 = svmul_n_s64_x (p0, z0, 127),
		z0 = svmul_x (p0, z0, 127))

/*
** mul_128_s64_x:
**	mov	(z[0-9]+\.d), #128
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_128_s64_x, svint64_t,
		z0 = svmul_n_s64_x (p0, z0, 128),
		z0 = svmul_x (p0, z0, 128))

/*
** mul_255_s64_x:
**	mov	(z[0-9]+\.d), #255
**	mul	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mul_255_s64_x, svint64_t,
		z0 = svmul_n_s64_x (p0, z0, 255),
		z0 = svmul_x (p0, z0, 255))

/*
** mul_m1_s64_x:
**	mul	z0\.d, z0\.d, #(-1|255)
**	ret
*/
TEST_UNIFORM_Z (mul_m1_s64_x, svint64_t,
		z0 = svmul_n_s64_x (p0, z0, -1),
		z0 = svmul_x (p0, z0, -1))

/*
** mul_m127_s64_x:
**	mul	z0\.d, z0\.d, #-127
**	ret
*/
TEST_UNIFORM_Z (mul_m127_s64_x, svint64_t,
		z0 = svmul_n_s64_x (p0, z0, -127),
		z0 = svmul_x (p0, z0, -127))

/*
** mul_m128_s64_x:
**	mul	z0\.d, z0\.d, #(-128|128)
**	ret
*/
TEST_UNIFORM_Z (mul_m128_s64_x, svint64_t,
		z0 = svmul_n_s64_x (p0, z0, -128),
		z0 = svmul_x (p0, z0, -128))
