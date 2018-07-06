/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** subr_u8_m:
**	subr	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_m, svuint8_t,
		z0 = svsubr_u8_m (p0, z0, z1),
		z0 = svsubr_m (p0, z0, z1))

/*
** subr_u8_m_prfx:
**	movprfx	z0, z1
**	subr	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_m_prfx, svuint8_t,
		z0 = svsubr_u8_m (p0, z1, z2),
		z0 = svsubr_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (subr_u8_m_bad_ra, svuint8_t,
		z1 = svsubr_u8_m (p0, z0, z1),
		z1 = svsubr_m (p0, z0, z1))

/*
** subr_w0_u8_m:
**	mov	(z[0-9]+\.b), w0
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_u8_m, svuint8_t, uint8_t,
		 z0 = svsubr_n_u8_m (p0, z0, x0),
		 z0 = svsubr_m (p0, z0, x0))

/*
** subr_w0_u8_m_prfx:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_u8_m_prfx, svuint8_t, uint8_t,
		 z0 = svsubr_n_u8_m (p0, z1, x0),
		 z0 = svsubr_m (p0, z1, x0))

/*
** subr_b0_u8_m:
**	mov	(z[0-9]+\.b), b0
**	subr	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_u8_m, svuint8_t, uint8_t,
		 z1 = svsubr_n_u8_m (p0, z1, d0),
		 z1 = svsubr_m (p0, z1, d0))

/*
** subr_b0_u8_m_prfx:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z2
**	subr	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_u8_m_prfx, svuint8_t, uint8_t,
		 z1 = svsubr_n_u8_m (p0, z2, d0),
		 z1 = svsubr_m (p0, z2, d0))

/*
** subr_1_u8_m:
**	mov	(z[0-9]+\.b), #1
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (subr_1_u8_m, svuint8_t,
		z0 = svsubr_n_u8_m (p0, z0, 1),
		z0 = svsubr_m (p0, z0, 1))

/*
** subr_1_u8_m_prfx:
**	mov	(z[0-9]+\.b), #1
**	movprfx	z0, z1
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (subr_1_u8_m_prfx, svuint8_t,
		z0 = svsubr_n_u8_m (p0, z1, 1),
		z0 = svsubr_m (p0, z1, 1))

/*
** subr_m1_u8_m:
**	mov	(z[0-9]+\.b), #(-1|255)
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_u8_m, svuint8_t,
		z0 = svsubr_n_u8_m (p0, z0, -1),
		z0 = svsubr_m (p0, z0, -1))

/*
** subr_u8_z_tied:
**	movprfx	z0\.b, p0/z, z0\.b
**	subr	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_z_tied, svuint8_t,
		z0 = svsubr_u8_z (p0, z0, z1),
		z0 = svsubr_z (p0, z0, z1))

/*
** subr_u8_z_rev:
**	movprfx	z1\.b, p0/z, z1\.b
**	sub	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_z_rev, svuint8_t,
		z1 = svsubr_u8_z (p0, z0, z1),
		z1 = svsubr_z (p0, z0, z1))

/*
** subr_u8_z_untied:
**	movprfx	z0\.b, p0/z, z2\.b
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_z_untied, svuint8_t,
		z0 = svsubr_u8_z (p0, z1, z2),
		z0 = svsubr_z (p0, z1, z2))

/*
** subr_w0_u8_z_tied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_u8_z_tied, svuint8_t, uint8_t,
		 z0 = svsubr_n_u8_z (p0, z0, x0),
		 z0 = svsubr_z (p0, z0, x0))

/*
** subr_w0_u8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, \1
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_u8_z_untied, svuint8_t, uint8_t,
		 z0 = svsubr_n_u8_z (p0, z1, x0),
		 z0 = svsubr_z (p0, z1, x0))

/*
** subr_b0_u8_z_tied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	subr	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_u8_z_tied, svuint8_t, uint8_t,
		 z1 = svsubr_n_u8_z (p0, z1, d0),
		 z1 = svsubr_z (p0, z1, d0))

/*
** subr_b0_u8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, \1
**	sub	z1\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_u8_z_untied, svuint8_t, uint8_t,
		 z1 = svsubr_n_u8_z (p0, z2, d0),
		 z1 = svsubr_z (p0, z2, d0))

/*
** subr_u8_x_z0:
**	sub	z0\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_x_z0, svuint8_t,
		z0 = svsubr_u8_x (p0, z0, z1),
		z0 = svsubr_x (p0, z0, z1))

/*
** subr_u8_x_z1:
**	sub	z1\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_x_z1, svuint8_t,
		z1 = svsubr_u8_x (p0, z0, z1),
		z1 = svsubr_x (p0, z0, z1))

/*
** subr_u8_x_z2:
**	sub	z2\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_u8_x_z2, svuint8_t,
		z2 = svsubr_u8_x (p0, z0, z1),
		z2 = svsubr_x (p0, z0, z1))

/*
** subr_w0_u8_x_z0:
**	mov	(z[0-9]+\.b), w0
**	sub	z0\.b, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_u8_x_z0, svuint8_t, uint8_t,
		 z0 = svsubr_n_u8_x (p0, z0, x0),
		 z0 = svsubr_x (p0, z0, x0))

/*
** subr_w0_u8_x_z1:
**	mov	(z[0-9]+\.b), w0
**	sub	z1\.b, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_u8_x_z1, svuint8_t, uint8_t,
		 z1 = svsubr_n_u8_x (p0, z0, x0),
		 z1 = svsubr_x (p0, z0, x0))

/*
** subr_b0_u8_x_z1:
**	mov	(z[0-9]+\.b), b0
**	sub	z1\.b, \1, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_u8_x_z1, svuint8_t, uint8_t,
		 z1 = svsubr_n_u8_x (p0, z1, d0),
		 z1 = svsubr_x (p0, z1, d0))

/*
** subr_b0_u8_x_z2:
**	mov	(z[0-9]+\.b), b0
**	sub	z2\.b, \1, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_u8_x_z2, svuint8_t, uint8_t,
		 z2 = svsubr_n_u8_x (p0, z1, d0),
		 z2 = svsubr_x (p0, z1, d0))

/*
** subr_1_u8_x:
**	subr	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (subr_1_u8_x, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z0, 1),
		z0 = svsubr_x (p0, z0, 1))

/*
** subr_1_u8_x_prfx:
**	movprfx	z0, z1
**	subr	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (subr_1_u8_x_prfx, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z1, 1),
		z0 = svsubr_x (p0, z1, 1))

/*
** subr_127_u8_x:
**	subr	z0\.b, z0\.b, #127
**	ret
*/
TEST_UNIFORM_Z (subr_127_u8_x, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z0, 127),
		z0 = svsubr_x (p0, z0, 127))

/*
** subr_128_u8_x:
**	subr	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (subr_128_u8_x, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z0, 128),
		z0 = svsubr_x (p0, z0, 128))

/*
** subr_255_u8_x:
**	subr	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (subr_255_u8_x, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z0, 255),
		z0 = svsubr_x (p0, z0, 255))

/*
** subr_m1_u8_x:
**	subr	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (subr_m1_u8_x, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z0, -1),
		z0 = svsubr_x (p0, z0, -1))

/*
** subr_m127_u8_x:
**	subr	z0\.b, z0\.b, #129
**	ret
*/
TEST_UNIFORM_Z (subr_m127_u8_x, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z0, -127),
		z0 = svsubr_x (p0, z0, -127))

/*
** subr_m128_u8_x:
**	subr	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (subr_m128_u8_x, svuint8_t,
		z0 = svsubr_n_u8_x (p0, z0, -128),
		z0 = svsubr_x (p0, z0, -128))
