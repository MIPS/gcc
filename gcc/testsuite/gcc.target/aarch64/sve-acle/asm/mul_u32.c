/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mul_u32_m_tied1:
**	mul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_m_tied1, svuint32_t,
		z0 = svmul_u32_m (p0, z0, z1),
		z0 = svmul_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mul_u32_m_tied2, svuint32_t,
		z1 = svmul_u32_m (p0, z0, z1),
		z1 = svmul_m (p0, z0, z1))

/*
** mul_u32_m_untied:
**	movprfx	z0, z1
**	mul	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_m_untied, svuint32_t,
		z0 = svmul_u32_m (p0, z1, z2),
		z0 = svmul_m (p0, z1, z2))

/*
** mul_w0_u32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u32_m_tied1, svuint32_t, uint32_t,
		 z0 = svmul_n_u32_m (p0, z0, x0),
		 z0 = svmul_m (p0, z0, x0))

/*
** mul_w0_u32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u32_m_untied, svuint32_t, uint32_t,
		 z0 = svmul_n_u32_m (p0, z1, x0),
		 z0 = svmul_m (p0, z1, x0))

/*
** mul_s0_u32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	mul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_u32_m_tied1, svuint32_t, uint32_t,
		 z1 = svmul_n_u32_m (p0, z1, d0),
		 z1 = svmul_m (p0, z1, d0))

/*
** mul_s0_u32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	mul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_u32_m_untied, svuint32_t, uint32_t,
		 z1 = svmul_n_u32_m (p0, z2, d0),
		 z1 = svmul_m (p0, z2, d0))

/*
** mul_2_u32_m_tied1:
**	mov	(z[0-9]+\.s), #2
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_u32_m_tied1, svuint32_t,
		z0 = svmul_n_u32_m (p0, z0, 2),
		z0 = svmul_m (p0, z0, 2))

/*
** mul_2_u32_m_untied:
**	mov	(z[0-9]+\.s), #2
**	movprfx	z0, z1
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_2_u32_m_untied, svuint32_t,
		z0 = svmul_n_u32_m (p0, z1, 2),
		z0 = svmul_m (p0, z1, 2))

/*
** mul_m1_u32_m:
**	mov	(z[0-9]+)\.b, #-1
**	mul	z0\.s, p0/m, z0\.s, \1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_m1_u32_m, svuint32_t,
		z0 = svmul_n_u32_m (p0, z0, -1),
		z0 = svmul_m (p0, z0, -1))

/*
** mul_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	mul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_z_tied1, svuint32_t,
		z0 = svmul_u32_z (p0, z0, z1),
		z0 = svmul_z (p0, z0, z1))

/*
** mul_u32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	mul	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_z_tied2, svuint32_t,
		z1 = svmul_u32_z (p0, z0, z1),
		z1 = svmul_z (p0, z0, z1))

/*
** mul_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	mul	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_z_untied, svuint32_t,
		z0 = svmul_u32_z (p0, z1, z2),
		z0 = svmul_z (p0, z1, z2))

/*
** mul_w0_u32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u32_z_tied1, svuint32_t, uint32_t,
		 z0 = svmul_n_u32_z (p0, z0, x0),
		 z0 = svmul_z (p0, z0, x0))

/*
** mul_w0_u32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z1\.s
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u32_z_untied, svuint32_t, uint32_t,
		 z0 = svmul_n_u32_z (p0, z1, x0),
		 z0 = svmul_z (p0, z1, x0))

/*
** mul_s0_u32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	mul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_u32_z_tied1, svuint32_t, uint32_t,
		 z1 = svmul_n_u32_z (p0, z1, d0),
		 z1 = svmul_z (p0, z1, d0))

/*
** mul_s0_u32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z2\.s
**	mul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_u32_z_untied, svuint32_t, uint32_t,
		 z1 = svmul_n_u32_z (p0, z2, d0),
		 z1 = svmul_z (p0, z2, d0))

/*
** mul_u32_x_tied1:
**	mul	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_x_tied1, svuint32_t,
		z0 = svmul_u32_x (p0, z0, z1),
		z0 = svmul_x (p0, z0, z1))

/*
** mul_u32_x_tied2:
**	mul	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_x_tied2, svuint32_t,
		z1 = svmul_u32_x (p0, z0, z1),
		z1 = svmul_x (p0, z0, z1))

/*
** mul_u32_x_untied:
**	movprfx	z2, z0
**	mul	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mul_u32_x_untied, svuint32_t,
		z2 = svmul_u32_x (p0, z0, z1),
		z2 = svmul_x (p0, z0, z1))

/*
** mul_w0_u32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u32_x_tied1, svuint32_t, uint32_t,
		 z0 = svmul_n_u32_x (p0, z0, x0),
		 z0 = svmul_x (p0, z0, x0))

/*
** mul_w0_u32_x_untied:
**	mov	z1\.s, w0
**	mul	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mul_w0_u32_x_untied, svuint32_t, uint32_t,
		 z1 = svmul_n_u32_x (p0, z0, x0),
		 z1 = svmul_x (p0, z0, x0))

/*
** mul_s0_u32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	mul	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_u32_x_tied1, svuint32_t, uint32_t,
		 z1 = svmul_n_u32_x (p0, z1, d0),
		 z1 = svmul_x (p0, z1, d0))

/*
** mul_s0_u32_x_untied:
**	mov	z2\.s, s0
**	mul	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (mul_s0_u32_x_untied, svuint32_t, uint32_t,
		 z2 = svmul_n_u32_x (p0, z1, d0),
		 z2 = svmul_x (p0, z1, d0))

/*
** mul_2_u32_x_tied1:
**	mul	z0\.s, z0\.s, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_u32_x_tied1, svuint32_t,
		z0 = svmul_n_u32_x (p0, z0, 2),
		z0 = svmul_x (p0, z0, 2))

/*
** mul_2_u32_x_untied:
**	mov	z0\.d, z1\.d
**	mul	z0\.s, z0\.s, #2
**	ret
*/
TEST_UNIFORM_Z (mul_2_u32_x_untied, svuint32_t,
		z0 = svmul_n_u32_x (p0, z1, 2),
		z0 = svmul_x (p0, z1, 2))

/*
** mul_127_u32_x:
**	mul	z0\.s, z0\.s, #127
**	ret
*/
TEST_UNIFORM_Z (mul_127_u32_x, svuint32_t,
		z0 = svmul_n_u32_x (p0, z0, 127),
		z0 = svmul_x (p0, z0, 127))

/*
** mul_128_u32_x:
**	mov	(z[0-9]+\.s), #128
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_128_u32_x, svuint32_t,
		z0 = svmul_n_u32_x (p0, z0, 128),
		z0 = svmul_x (p0, z0, 128))

/*
** mul_255_u32_x:
**	mov	(z[0-9]+\.s), #255
**	mul	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mul_255_u32_x, svuint32_t,
		z0 = svmul_n_u32_x (p0, z0, 255),
		z0 = svmul_x (p0, z0, 255))

/*
** mul_m1_u32_x:
**	mul	z0\.s, z0\.s, #(-1|255)
**	ret
*/
TEST_UNIFORM_Z (mul_m1_u32_x, svuint32_t,
		z0 = svmul_n_u32_x (p0, z0, -1),
		z0 = svmul_x (p0, z0, -1))

/*
** mul_m127_u32_x:
**	mul	z0\.s, z0\.s, #-127
**	ret
*/
TEST_UNIFORM_Z (mul_m127_u32_x, svuint32_t,
		z0 = svmul_n_u32_x (p0, z0, -127),
		z0 = svmul_x (p0, z0, -127))

/*
** mul_m128_u32_x:
**	mul	z0\.s, z0\.s, #(-128|128)
**	ret
*/
TEST_UNIFORM_Z (mul_m128_u32_x, svuint32_t,
		z0 = svmul_n_u32_x (p0, z0, -128),
		z0 = svmul_x (p0, z0, -128))
