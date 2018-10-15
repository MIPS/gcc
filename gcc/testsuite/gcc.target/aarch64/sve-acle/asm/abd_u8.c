/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** abd_u8_m_tied1:
**	uabd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_m_tied1, svuint8_t,
		z0 = svabd_u8_m (p0, z0, z1),
		z0 = svabd_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (abd_u8_m_tied2, svuint8_t,
		z1 = svabd_u8_m (p0, z0, z1),
		z1 = svabd_m (p0, z0, z1))

/*
** abd_u8_m_untied:
**	movprfx	z0, z1
**	uabd	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_m_untied, svuint8_t,
		z0 = svabd_u8_m (p0, z1, z2),
		z0 = svabd_m (p0, z1, z2))

/*
** abd_w0_u8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	uabd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_u8_m_tied1, svuint8_t, uint8_t,
		 z0 = svabd_n_u8_m (p0, z0, x0),
		 z0 = svabd_m (p0, z0, x0))

/*
** abd_w0_u8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	uabd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_u8_m_untied, svuint8_t, uint8_t,
		 z0 = svabd_n_u8_m (p0, z1, x0),
		 z0 = svabd_m (p0, z1, x0))

/*
** abd_b0_u8_m_tied1:
**	mov	(z[0-9]+\.b), b0
**	uabd	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_b0_u8_m_tied1, svuint8_t, uint8_t,
		 z1 = svabd_n_u8_m (p0, z1, d0),
		 z1 = svabd_m (p0, z1, d0))

/*
** abd_b0_u8_m_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z2
**	uabd	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_b0_u8_m_untied, svuint8_t, uint8_t,
		 z1 = svabd_n_u8_m (p0, z2, d0),
		 z1 = svabd_m (p0, z2, d0))

/*
** abd_1_u8_m_tied1:
**	mov	(z[0-9]+\.b), #1
**	uabd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (abd_1_u8_m_tied1, svuint8_t,
		z0 = svabd_n_u8_m (p0, z0, 1),
		z0 = svabd_m (p0, z0, 1))

/*
** abd_1_u8_m_untied:
**	mov	(z[0-9]+\.b), #1
**	movprfx	z0, z1
**	uabd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (abd_1_u8_m_untied, svuint8_t,
		z0 = svabd_n_u8_m (p0, z1, 1),
		z0 = svabd_m (p0, z1, 1))

/*
** abd_u8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	uabd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_z_tied1, svuint8_t,
		z0 = svabd_u8_z (p0, z0, z1),
		z0 = svabd_z (p0, z0, z1))

/*
** abd_u8_z_tied2:
**	movprfx	z1\.b, p0/z, z1\.b
**	uabd	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_z_tied2, svuint8_t,
		z1 = svabd_u8_z (p0, z0, z1),
		z1 = svabd_z (p0, z0, z1))

/*
** abd_u8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	uabd	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_z_untied, svuint8_t,
		z0 = svabd_u8_z (p0, z1, z2),
		z0 = svabd_z (p0, z1, z2))

/*
** abd_w0_u8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	uabd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_u8_z_tied1, svuint8_t, uint8_t,
		 z0 = svabd_n_u8_z (p0, z0, x0),
		 z0 = svabd_z (p0, z0, x0))

/*
** abd_w0_u8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, \1
**	uabd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_u8_z_untied, svuint8_t, uint8_t,
		 z0 = svabd_n_u8_z (p0, z1, x0),
		 z0 = svabd_z (p0, z1, x0))

/*
** abd_b0_u8_z_tied1:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	uabd	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_b0_u8_z_tied1, svuint8_t, uint8_t,
		 z1 = svabd_n_u8_z (p0, z1, d0),
		 z1 = svabd_z (p0, z1, d0))

/*
** abd_b0_u8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, \1
**	uabd	z1\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_ZS (abd_b0_u8_z_untied, svuint8_t, uint8_t,
		 z1 = svabd_n_u8_z (p0, z2, d0),
		 z1 = svabd_z (p0, z2, d0))

/*
** abd_u8_x_tied1:
**	uabd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_x_tied1, svuint8_t,
		z0 = svabd_u8_x (p0, z0, z1),
		z0 = svabd_x (p0, z0, z1))

/*
** abd_u8_x_tied2:
**	uabd	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_x_tied2, svuint8_t,
		z1 = svabd_u8_x (p0, z0, z1),
		z1 = svabd_x (p0, z0, z1))

/*
** abd_u8_x_untied:
**	movprfx	z2, z0
**	uabd	z2\.b, p0/m, z2\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (abd_u8_x_untied, svuint8_t,
		z2 = svabd_u8_x (p0, z0, z1),
		z2 = svabd_x (p0, z0, z1))

/*
** abd_w0_u8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	uabd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_u8_x_tied1, svuint8_t, uint8_t,
		 z0 = svabd_n_u8_x (p0, z0, x0),
		 z0 = svabd_x (p0, z0, x0))

/*
** abd_w0_u8_x_untied:
**	mov	z1\.b, w0
**	uabd	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (abd_w0_u8_x_untied, svuint8_t, uint8_t,
		 z1 = svabd_n_u8_x (p0, z0, x0),
		 z1 = svabd_x (p0, z0, x0))

/*
** abd_b0_u8_x_tied1:
**	mov	(z[0-9]+\.b), b0
**	uabd	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (abd_b0_u8_x_tied1, svuint8_t, uint8_t,
		 z1 = svabd_n_u8_x (p0, z1, d0),
		 z1 = svabd_x (p0, z1, d0))

/*
** abd_b0_u8_x_untied:
**	mov	z2\.b, b0
**	uabd	z2\.b, p0/m, z2\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (abd_b0_u8_x_untied, svuint8_t, uint8_t,
		 z2 = svabd_n_u8_x (p0, z1, d0),
		 z2 = svabd_x (p0, z1, d0))

/*
** abd_1_u8_x_tied1:
**	mov	(z[0-9]+\.b), #1
**	uabd	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (abd_1_u8_x_tied1, svuint8_t,
		z0 = svabd_n_u8_x (p0, z0, 1),
		z0 = svabd_x (p0, z0, 1))

/*
** abd_1_u8_x_untied:
**	mov	z0\.b, #1
**	uabd	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (abd_1_u8_x_untied, svuint8_t,
		z0 = svabd_n_u8_x (p0, z1, 1),
		z0 = svabd_x (p0, z1, 1))

