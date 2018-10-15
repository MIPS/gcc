/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mul_u8_m_tied1:
**	mul	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_m_tied1, svuint8_t,
		z0 = svmul_u8_m (p0, z0, z1),
		z0 = svmul_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mul_u8_m_tied2, svuint8_t,
		z1 = svmul_u8_m (p0, z0, z1),
		z1 = svmul_m (p0, z0, z1))

/*
** mul_u8_m_untied:
**	movprfx	z0, z1
**	mul	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_m_untied, svuint8_t,
		z0 = svmul_u8_m (p0, z1, z2),
		z0 = svmul_m (p0, z1, z2))

/*
** mul_w0_u8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u8_m_tied1, svuint8_t, uint8_t,
		 z0 = svmul_n_u8_m (p0, z0, x0),
		 z0 = svmul_m (p0, z0, x0))

/*
** mul_w0_u8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u8_m_untied, svuint8_t, uint8_t,
		 z0 = svmul_n_u8_m (p0, z1, x0),
		 z0 = svmul_m (p0, z1, x0))

/*
** mul_b0_u8_m_tied1:
**	mov	(z[0-9]+\.b), b0
**	mul	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_b0_u8_m_tied1, svuint8_t, uint8_t,
		 z1 = svmul_n_u8_m (p0, z1, d0),
		 z1 = svmul_m (p0, z1, d0))

/*
** mul_b0_u8_m_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z2
**	mul	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_b0_u8_m_untied, svuint8_t, uint8_t,
		 z1 = svmul_n_u8_m (p0, z2, d0),
		 z1 = svmul_m (p0, z2, d0))

/*
** mul_2_u8_m_tied1:
**	mov	(z[0-9]+\.b), #2
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_u8_m_tied1, svuint8_t,
		z0 = svmul_n_u8_m (p0, z0, 2),
		z0 = svmul_m (p0, z0, 2))

/*
** mul_2_u8_m_untied:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0, z1
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_u8_m_untied, svuint8_t,
		z0 = svmul_n_u8_m (p0, z1, 2),
		z0 = svmul_m (p0, z1, 2))

/*
** mul_m1_u8_m:
**	mov	(z[0-9]+\.b), #(-1|255)
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mul_m1_u8_m, svuint8_t,
		z0 = svmul_n_u8_m (p0, z0, -1),
		z0 = svmul_m (p0, z0, -1))

/*
** mul_u8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	mul	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_z_tied1, svuint8_t,
		z0 = svmul_u8_z (p0, z0, z1),
		z0 = svmul_z (p0, z0, z1))

/*
** mul_u8_z_tied2:
**	movprfx	z1\.b, p0/z, z1\.b
**	mul	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_z_tied2, svuint8_t,
		z1 = svmul_u8_z (p0, z0, z1),
		z1 = svmul_z (p0, z0, z1))

/*
** mul_u8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	mul	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_z_untied, svuint8_t,
		z0 = svmul_u8_z (p0, z1, z2),
		z0 = svmul_z (p0, z1, z2))

/*
** mul_w0_u8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u8_z_tied1, svuint8_t, uint8_t,
		 z0 = svmul_n_u8_z (p0, z0, x0),
		 z0 = svmul_z (p0, z0, x0))

/*
** mul_w0_u8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z1\.b
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u8_z_untied, svuint8_t, uint8_t,
		 z0 = svmul_n_u8_z (p0, z1, x0),
		 z0 = svmul_z (p0, z1, x0))

/*
** mul_b0_u8_z_tied1:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	mul	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_b0_u8_z_tied1, svuint8_t, uint8_t,
		 z1 = svmul_n_u8_z (p0, z1, d0),
		 z1 = svmul_z (p0, z1, d0))

/*
** mul_b0_u8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z2\.b
**	mul	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_b0_u8_z_untied, svuint8_t, uint8_t,
		 z1 = svmul_n_u8_z (p0, z2, d0),
		 z1 = svmul_z (p0, z2, d0))

/*
** mul_u8_x_tied1:
**	mul	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_x_tied1, svuint8_t,
		z0 = svmul_u8_x (p0, z0, z1),
		z0 = svmul_x (p0, z0, z1))

/*
** mul_u8_x_tied2:
**	mul	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_x_tied2, svuint8_t,
		z1 = svmul_u8_x (p0, z0, z1),
		z1 = svmul_x (p0, z0, z1))

/*
** mul_u8_x_untied:
**	movprfx	z2, z0
**	mul	z2\.b, p0/m, z2\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mul_u8_x_untied, svuint8_t,
		z2 = svmul_u8_x (p0, z0, z1),
		z2 = svmul_x (p0, z0, z1))

/*
** mul_w0_u8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	mul	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u8_x_tied1, svuint8_t, uint8_t,
		 z0 = svmul_n_u8_x (p0, z0, x0),
		 z0 = svmul_x (p0, z0, x0))

/*
** mul_w0_u8_x_untied:
**	mov	z1\.b, w0
**	mul	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u8_x_untied, svuint8_t, uint8_t,
		 z1 = svmul_n_u8_x (p0, z0, x0),
		 z1 = svmul_x (p0, z0, x0))

/*
** mul_b0_u8_x_tied1:
**	mov	(z[0-9]+\.b), b0
**	mul	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_b0_u8_x_tied1, svuint8_t, uint8_t,
		 z1 = svmul_n_u8_x (p0, z1, d0),
		 z1 = svmul_x (p0, z1, d0))

/*
** mul_b0_u8_x_untied:
**	mov	z2\.b, b0
**	mul	z2\.b, p0/m, z2\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (mul_b0_u8_x_untied, svuint8_t, uint8_t,
		 z2 = svmul_n_u8_x (p0, z1, d0),
		 z2 = svmul_x (p0, z1, d0))

/*
** mul_2_u8_x_tied1:
**	mul	z0\.b, z0\.b, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_u8_x_tied1, svuint8_t,
		z0 = svmul_n_u8_x (p0, z0, 2),
		z0 = svmul_x (p0, z0, 2))

/*
** mul_2_u8_x_untied:
**	mov	z0\.d, z1\.d
**	mul	z0\.b, z0\.b, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_u8_x_untied, svuint8_t,
		z0 = svmul_n_u8_x (p0, z1, 2),
		z0 = svmul_x (p0, z1, 2))

/*
** mul_127_u8_x:
**	mul	z0\.b, z0\.b, #127
**	ret
*/
TEST_UNIFORM_Z (mul_127_u8_x, svuint8_t,
		z0 = svmul_n_u8_x (p0, z0, 127),
		z0 = svmul_x (p0, z0, 127))

/*
** mul_128_u8_x:
**	mul	z0\.b, z0\.b, #(128|-128)
**	ret
*/
TEST_UNIFORM_Z (mul_128_u8_x, svuint8_t,
		z0 = svmul_n_u8_x (p0, z0, 128),
		z0 = svmul_x (p0, z0, 128))

/*
** mul_255_u8_x:
**	mul	z0\.b, z0\.b, #(-1|255)
**	ret
*/
TEST_UNIFORM_Z (mul_255_u8_x, svuint8_t,
		z0 = svmul_n_u8_x (p0, z0, 255),
		z0 = svmul_x (p0, z0, 255))

/*
** mul_m1_u8_x:
**	mul	z0\.b, z0\.b, #(-1|255)
**	ret
*/
TEST_UNIFORM_Z (mul_m1_u8_x, svuint8_t,
		z0 = svmul_n_u8_x (p0, z0, -1),
		z0 = svmul_x (p0, z0, -1))

/*
** mul_m127_u8_x:
**	mul	z0\.b, z0\.b, #-127
**	ret
*/
TEST_UNIFORM_Z (mul_m127_u8_x, svuint8_t,
		z0 = svmul_n_u8_x (p0, z0, -127),
		z0 = svmul_x (p0, z0, -127))

/*
** mul_m128_u8_x:
**	mul	z0\.b, z0\.b, #(-128|128)
**	ret
*/
TEST_UNIFORM_Z (mul_m128_u8_x, svuint8_t,
		z0 = svmul_n_u8_x (p0, z0, -128),
		z0 = svmul_x (p0, z0, -128))
