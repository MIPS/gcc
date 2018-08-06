/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** subr_s8_m_tied1:
**	subr	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_m_tied1, svint8_t,
		z0 = svsubr_s8_m (p0, z0, z1),
		z0 = svsubr_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (subr_s8_m_tied2, svint8_t,
		z1 = svsubr_s8_m (p0, z0, z1),
		z1 = svsubr_m (p0, z0, z1))

/*
** subr_s8_m_untied:
**	movprfx	z0, z1
**	subr	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_m_untied, svint8_t,
		z0 = svsubr_s8_m (p0, z1, z2),
		z0 = svsubr_m (p0, z1, z2))

/*
** subr_w0_s8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s8_m_tied1, svint8_t, int8_t,
		 z0 = svsubr_n_s8_m (p0, z0, x0),
		 z0 = svsubr_m (p0, z0, x0))

/*
** subr_w0_s8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s8_m_untied, svint8_t, int8_t,
		 z0 = svsubr_n_s8_m (p0, z1, x0),
		 z0 = svsubr_m (p0, z1, x0))

/*
** subr_b0_s8_m_tied1:
**	mov	(z[0-9]+\.b), b0
**	subr	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_s8_m_tied1, svint8_t, int8_t,
		 z1 = svsubr_n_s8_m (p0, z1, d0),
		 z1 = svsubr_m (p0, z1, d0))

/*
** subr_b0_s8_m_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z2
**	subr	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_s8_m_untied, svint8_t, int8_t,
		 z1 = svsubr_n_s8_m (p0, z2, d0),
		 z1 = svsubr_m (p0, z2, d0))

/*
** subr_1_s8_m_tied1:
**	mov	(z[0-9]+\.b), #1
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s8_m_tied1, svint8_t,
		z0 = svsubr_n_s8_m (p0, z0, 1),
		z0 = svsubr_m (p0, z0, 1))

/*
** subr_1_s8_m_untied:
**	mov	(z[0-9]+\.b), #1
**	movprfx	z0, z1
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s8_m_untied, svint8_t,
		z0 = svsubr_n_s8_m (p0, z1, 1),
		z0 = svsubr_m (p0, z1, 1))

/*
** subr_m1_s8_m:
**	mov	(z[0-9]+\.b), #(-1|255)
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m1_s8_m, svint8_t,
		z0 = svsubr_n_s8_m (p0, z0, -1),
		z0 = svsubr_m (p0, z0, -1))

/*
** subr_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	subr	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_z_tied1, svint8_t,
		z0 = svsubr_s8_z (p0, z0, z1),
		z0 = svsubr_z (p0, z0, z1))

/*
** subr_s8_z_tied2:
**	movprfx	z1\.b, p0/z, z1\.b
**	sub	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_z_tied2, svint8_t,
		z1 = svsubr_s8_z (p0, z0, z1),
		z1 = svsubr_z (p0, z0, z1))

/*
** subr_s8_z_untied:
**	movprfx	z0\.b, p0/z, z2\.b
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_z_untied, svint8_t,
		z0 = svsubr_s8_z (p0, z1, z2),
		z0 = svsubr_z (p0, z1, z2))

/*
** subr_w0_s8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	subr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s8_z_tied1, svint8_t, int8_t,
		 z0 = svsubr_n_s8_z (p0, z0, x0),
		 z0 = svsubr_z (p0, z0, x0))

/*
** subr_w0_s8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, \1
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s8_z_untied, svint8_t, int8_t,
		 z0 = svsubr_n_s8_z (p0, z1, x0),
		 z0 = svsubr_z (p0, z1, x0))

/*
** subr_b0_s8_z_tied1:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	subr	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_s8_z_tied1, svint8_t, int8_t,
		 z1 = svsubr_n_s8_z (p0, z1, d0),
		 z1 = svsubr_z (p0, z1, d0))

/*
** subr_b0_s8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, \1
**	sub	z1\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_s8_z_untied, svint8_t, int8_t,
		 z1 = svsubr_n_s8_z (p0, z2, d0),
		 z1 = svsubr_z (p0, z2, d0))

/*
** subr_s8_x_tied1:
**	sub	z0\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_x_tied1, svint8_t,
		z0 = svsubr_s8_x (p0, z0, z1),
		z0 = svsubr_x (p0, z0, z1))

/*
** subr_s8_x_tied2:
**	sub	z1\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_x_tied2, svint8_t,
		z1 = svsubr_s8_x (p0, z0, z1),
		z1 = svsubr_x (p0, z0, z1))

/*
** subr_s8_x_untied:
**	sub	z2\.b, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (subr_s8_x_untied, svint8_t,
		z2 = svsubr_s8_x (p0, z0, z1),
		z2 = svsubr_x (p0, z0, z1))

/*
** subr_w0_s8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	sub	z0\.b, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s8_x_tied1, svint8_t, int8_t,
		 z0 = svsubr_n_s8_x (p0, z0, x0),
		 z0 = svsubr_x (p0, z0, x0))

/*
** subr_w0_s8_x_untied:
**	mov	(z[0-9]+\.b), w0
**	sub	z1\.b, \1, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s8_x_untied, svint8_t, int8_t,
		 z1 = svsubr_n_s8_x (p0, z0, x0),
		 z1 = svsubr_x (p0, z0, x0))

/*
** subr_b0_s8_x_tied1:
**	mov	(z[0-9]+\.b), b0
**	sub	z1\.b, \1, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_s8_x_tied1, svint8_t, int8_t,
		 z1 = svsubr_n_s8_x (p0, z1, d0),
		 z1 = svsubr_x (p0, z1, d0))

/*
** subr_b0_s8_x_untied:
**	mov	(z[0-9]+\.b), b0
**	sub	z2\.b, \1, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (subr_b0_s8_x_untied, svint8_t, int8_t,
		 z2 = svsubr_n_s8_x (p0, z1, d0),
		 z2 = svsubr_x (p0, z1, d0))

/*
** subr_1_s8_x_tied1:
**	subr	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s8_x_tied1, svint8_t,
		z0 = svsubr_n_s8_x (p0, z0, 1),
		z0 = svsubr_x (p0, z0, 1))

/*
** subr_1_s8_x_untied:
**	movprfx	z0, z1
**	subr	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s8_x_untied, svint8_t,
		z0 = svsubr_n_s8_x (p0, z1, 1),
		z0 = svsubr_x (p0, z1, 1))

/*
** subr_127_s8_x:
**	subr	z0\.b, z0\.b, #127
**	ret
*/
TEST_UNIFORM_Z (subr_127_s8_x, svint8_t,
		z0 = svsubr_n_s8_x (p0, z0, 127),
		z0 = svsubr_x (p0, z0, 127))

/*
** subr_128_s8_x:
**	subr	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (subr_128_s8_x, svint8_t,
		z0 = svsubr_n_s8_x (p0, z0, 128),
		z0 = svsubr_x (p0, z0, 128))

/*
** subr_255_s8_x:
**	subr	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (subr_255_s8_x, svint8_t,
		z0 = svsubr_n_s8_x (p0, z0, 255),
		z0 = svsubr_x (p0, z0, 255))

/*
** subr_m1_s8_x:
**	subr	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (subr_m1_s8_x, svint8_t,
		z0 = svsubr_n_s8_x (p0, z0, -1),
		z0 = svsubr_x (p0, z0, -1))

/*
** subr_m127_s8_x:
**	subr	z0\.b, z0\.b, #129
**	ret
*/
TEST_UNIFORM_Z (subr_m127_s8_x, svint8_t,
		z0 = svsubr_n_s8_x (p0, z0, -127),
		z0 = svsubr_x (p0, z0, -127))

/*
** subr_m128_s8_x:
**	subr	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (subr_m128_s8_x, svint8_t,
		z0 = svsubr_n_s8_x (p0, z0, -128),
		z0 = svsubr_x (p0, z0, -128))
