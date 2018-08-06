/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** sub_u8_m_tied1:
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_m_tied1, svuint8_t,
		z0 = svsub_u8_m (p0, z0, z1),
		z0 = svsub_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (sub_u8_m_tied2, svuint8_t,
		z1 = svsub_u8_m (p0, z0, z1),
		z1 = svsub_m (p0, z0, z1))

/*
** sub_u8_m_untied:
**	movprfx	z0, z1
**	sub	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_m_untied, svuint8_t,
		z0 = svsub_u8_m (p0, z1, z2),
		z0 = svsub_m (p0, z1, z2))

/*
** sub_w0_u8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_u8_m_tied1, svuint8_t, uint8_t,
		 z0 = svsub_n_u8_m (p0, z0, x0),
		 z0 = svsub_m (p0, z0, x0))

/*
** sub_w0_u8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_u8_m_untied, svuint8_t, uint8_t,
		 z0 = svsub_n_u8_m (p0, z1, x0),
		 z0 = svsub_m (p0, z1, x0))

/*
** sub_b0_u8_m_tied1:
**	mov	(z[0-9]+\.b), b0
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_u8_m_tied1, svuint8_t, uint8_t,
		 z1 = svsub_n_u8_m (p0, z1, d0),
		 z1 = svsub_m (p0, z1, d0))

/*
** sub_b0_u8_m_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z2
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_u8_m_untied, svuint8_t, uint8_t,
		 z1 = svsub_n_u8_m (p0, z2, d0),
		 z1 = svsub_m (p0, z2, d0))

/*
** sub_1_u8_m_tied1:
**	mov	(z[0-9]+\.b), #(-1|255)
**	add	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (sub_1_u8_m_tied1, svuint8_t,
		z0 = svsub_n_u8_m (p0, z0, 1),
		z0 = svsub_m (p0, z0, 1))

/*
** sub_1_u8_m_untied:
**	mov	(z[0-9]+\.b), #(-1|255)
**	movprfx	z0, z1
**	add	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (sub_1_u8_m_untied, svuint8_t,
		z0 = svsub_n_u8_m (p0, z1, 1),
		z0 = svsub_m (p0, z1, 1))

/*
** sub_m1_u8_m:
**	mov	(z[0-9]+\.b), #1
**	add	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (sub_m1_u8_m, svuint8_t,
		z0 = svsub_n_u8_m (p0, z0, -1),
		z0 = svsub_m (p0, z0, -1))

/*
** sub_u8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_z_tied1, svuint8_t,
		z0 = svsub_u8_z (p0, z0, z1),
		z0 = svsub_z (p0, z0, z1))

/*
** sub_u8_z_tied2:
**	movprfx	z1\.b, p0/z, z1\.b
**	subr	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_z_tied2, svuint8_t,
		z1 = svsub_u8_z (p0, z0, z1),
		z1 = svsub_z (p0, z0, z1))

/*
** sub_u8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	sub	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_z_untied, svuint8_t,
		z0 = svsub_u8_z (p0, z1, z2),
		z0 = svsub_z (p0, z1, z2))

/*
** sub_w0_u8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_u8_z_tied1, svuint8_t, uint8_t,
		 z0 = svsub_n_u8_z (p0, z0, x0),
		 z0 = svsub_z (p0, z0, x0))

/*
** sub_w0_u8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z1\.b
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_u8_z_untied, svuint8_t, uint8_t,
		 z0 = svsub_n_u8_z (p0, z1, x0),
		 z0 = svsub_z (p0, z1, x0))

/*
** sub_b0_u8_z_tied1:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_u8_z_tied1, svuint8_t, uint8_t,
		 z1 = svsub_n_u8_z (p0, z1, d0),
		 z1 = svsub_z (p0, z1, d0))

/*
** sub_b0_u8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z2\.b
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_u8_z_untied, svuint8_t, uint8_t,
		 z1 = svsub_n_u8_z (p0, z2, d0),
		 z1 = svsub_z (p0, z2, d0))

/*
** sub_u8_x_tied1:
**	sub	z0\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_x_tied1, svuint8_t,
		z0 = svsub_u8_x (p0, z0, z1),
		z0 = svsub_x (p0, z0, z1))

/*
** sub_u8_x_tied2:
**	sub	z1\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_x_tied2, svuint8_t,
		z1 = svsub_u8_x (p0, z0, z1),
		z1 = svsub_x (p0, z0, z1))

/*
** sub_u8_x_untied:
**	sub	z2\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_u8_x_untied, svuint8_t,
		z2 = svsub_u8_x (p0, z0, z1),
		z2 = svsub_x (p0, z0, z1))

/*
** sub_w0_u8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	sub	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_u8_x_tied1, svuint8_t, uint8_t,
		 z0 = svsub_n_u8_x (p0, z0, x0),
		 z0 = svsub_x (p0, z0, x0))

/*
** sub_w0_u8_x_untied:
**	mov	(z[0-9]+\.b), w0
**	sub	z1\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_u8_x_untied, svuint8_t, uint8_t,
		 z1 = svsub_n_u8_x (p0, z0, x0),
		 z1 = svsub_x (p0, z0, x0))

/*
** sub_b0_u8_x_tied1:
**	mov	(z[0-9]+\.b), b0
**	sub	z1\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_u8_x_tied1, svuint8_t, uint8_t,
		 z1 = svsub_n_u8_x (p0, z1, d0),
		 z1 = svsub_x (p0, z1, d0))

/*
** sub_b0_u8_x_untied:
**	mov	(z[0-9]+\.b), b0
**	sub	z2\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_u8_x_untied, svuint8_t, uint8_t,
		 z2 = svsub_n_u8_x (p0, z1, d0),
		 z2 = svsub_x (p0, z1, d0))

/*
** sub_1_u8_x_tied1:
**	add	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (sub_1_u8_x_tied1, svuint8_t,
		z0 = svsub_n_u8_x (p0, z0, 1),
		z0 = svsub_x (p0, z0, 1))

/*
** sub_1_u8_x_untied:
**	movprfx	z0, z1
**	add	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (sub_1_u8_x_untied, svuint8_t,
		z0 = svsub_n_u8_x (p0, z1, 1),
		z0 = svsub_x (p0, z1, 1))

/*
** sub_127_u8_x:
**	add	z0\.b, z0\.b, #129
**	ret
*/
TEST_UNIFORM_Z (sub_127_u8_x, svuint8_t,
		z0 = svsub_n_u8_x (p0, z0, 127),
		z0 = svsub_x (p0, z0, 127))

/*
** sub_128_u8_x:
**	add	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (sub_128_u8_x, svuint8_t,
		z0 = svsub_n_u8_x (p0, z0, 128),
		z0 = svsub_x (p0, z0, 128))

/*
** sub_255_u8_x:
**	add	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (sub_255_u8_x, svuint8_t,
		z0 = svsub_n_u8_x (p0, z0, 255),
		z0 = svsub_x (p0, z0, 255))

/*
** sub_m1_u8_x:
**	add	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (sub_m1_u8_x, svuint8_t,
		z0 = svsub_n_u8_x (p0, z0, -1),
		z0 = svsub_x (p0, z0, -1))

/*
** sub_m127_u8_x:
**	add	z0\.b, z0\.b, #127
**	ret
*/
TEST_UNIFORM_Z (sub_m127_u8_x, svuint8_t,
		z0 = svsub_n_u8_x (p0, z0, -127),
		z0 = svsub_x (p0, z0, -127))

/*
** sub_m128_u8_x:
**	add	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (sub_m128_u8_x, svuint8_t,
		z0 = svsub_n_u8_x (p0, z0, -128),
		z0 = svsub_x (p0, z0, -128))
