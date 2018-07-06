/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** sub_s8_m:
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_m, svint8_t,
		z0 = svsub_s8_m (p0, z0, z1),
		z0 = svsub_m (p0, z0, z1))

/*
** sub_s8_m_prfx:
**	movprfx	z0, z1
**	sub	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_m_prfx, svint8_t,
		z0 = svsub_s8_m (p0, z1, z2),
		z0 = svsub_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (sub_s8_m_bad_ra, svint8_t,
		z1 = svsub_s8_m (p0, z0, z1),
		z1 = svsub_m (p0, z0, z1))

/*
** sub_w0_s8_m:
**	mov	(z[0-9]+\.b), w0
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_s8_m, svint8_t, int8_t,
		 z0 = svsub_n_s8_m (p0, z0, x0),
		 z0 = svsub_m (p0, z0, x0))

/*
** sub_w0_s8_m_prfx:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_s8_m_prfx, svint8_t, int8_t,
		 z0 = svsub_n_s8_m (p0, z1, x0),
		 z0 = svsub_m (p0, z1, x0))

/*
** sub_b0_s8_m:
**	mov	(z[0-9]+\.b), b0
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_s8_m, svint8_t, int8_t,
		 z1 = svsub_n_s8_m (p0, z1, d0),
		 z1 = svsub_m (p0, z1, d0))

/*
** sub_b0_s8_m_prfx:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z2
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_s8_m_prfx, svint8_t, int8_t,
		 z1 = svsub_n_s8_m (p0, z2, d0),
		 z1 = svsub_m (p0, z2, d0))

/*
** sub_1_s8_m:
**	mov	(z[0-9]+\.b), #(-1|255)
**	add	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (sub_1_s8_m, svint8_t,
		z0 = svsub_n_s8_m (p0, z0, 1),
		z0 = svsub_m (p0, z0, 1))

/*
** sub_1_s8_m_prfx:
**	mov	(z[0-9]+\.b), #(-1|255)
**	movprfx	z0, z1
**	add	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (sub_1_s8_m_prfx, svint8_t,
		z0 = svsub_n_s8_m (p0, z1, 1),
		z0 = svsub_m (p0, z1, 1))

/*
** sub_m1_s8_m:
**	mov	(z[0-9]+\.b), #1
**	add	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (sub_m1_s8_m, svint8_t,
		z0 = svsub_n_s8_m (p0, z0, -1),
		z0 = svsub_m (p0, z0, -1))

/*
** sub_s8_z_tied:
**	movprfx	z0\.b, p0/z, z0\.b
**	sub	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_z_tied, svint8_t,
		z0 = svsub_s8_z (p0, z0, z1),
		z0 = svsub_z (p0, z0, z1))

/*
** sub_s8_z_rev:
**	movprfx	z1\.b, p0/z, z1\.b
**	subr	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_z_rev, svint8_t,
		z1 = svsub_s8_z (p0, z0, z1),
		z1 = svsub_z (p0, z0, z1))

/*
** sub_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	sub	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_z_untied, svint8_t,
		z0 = svsub_s8_z (p0, z1, z2),
		z0 = svsub_z (p0, z1, z2))

/*
** sub_w0_s8_z_tied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_s8_z_tied, svint8_t, int8_t,
		 z0 = svsub_n_s8_z (p0, z0, x0),
		 z0 = svsub_z (p0, z0, x0))

/*
** sub_w0_s8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z1\.b
**	sub	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_s8_z_untied, svint8_t, int8_t,
		 z0 = svsub_n_s8_z (p0, z1, x0),
		 z0 = svsub_z (p0, z1, x0))

/*
** sub_b0_s8_z_tied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_s8_z_tied, svint8_t, int8_t,
		 z1 = svsub_n_s8_z (p0, z1, d0),
		 z1 = svsub_z (p0, z1, d0))

/*
** sub_b0_s8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z2\.b
**	sub	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_s8_z_untied, svint8_t, int8_t,
		 z1 = svsub_n_s8_z (p0, z2, d0),
		 z1 = svsub_z (p0, z2, d0))

/*
** sub_s8_x_z0:
**	sub	z0\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_x_z0, svint8_t,
		z0 = svsub_s8_x (p0, z0, z1),
		z0 = svsub_x (p0, z0, z1))

/*
** sub_s8_x_z1:
**	sub	z1\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_x_z1, svint8_t,
		z1 = svsub_s8_x (p0, z0, z1),
		z1 = svsub_x (p0, z0, z1))

/*
** sub_s8_x_z2:
**	sub	z2\.b, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (sub_s8_x_z2, svint8_t,
		z2 = svsub_s8_x (p0, z0, z1),
		z2 = svsub_x (p0, z0, z1))

/*
** sub_w0_s8_x_z0:
**	mov	(z[0-9]+\.b), w0
**	sub	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_s8_x_z0, svint8_t, int8_t,
		 z0 = svsub_n_s8_x (p0, z0, x0),
		 z0 = svsub_x (p0, z0, x0))

/*
** sub_w0_s8_x_z1:
**	mov	(z[0-9]+\.b), w0
**	sub	z1\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_w0_s8_x_z1, svint8_t, int8_t,
		 z1 = svsub_n_s8_x (p0, z0, x0),
		 z1 = svsub_x (p0, z0, x0))

/*
** sub_b0_s8_x_z1:
**	mov	(z[0-9]+\.b), b0
**	sub	z1\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_s8_x_z1, svint8_t, int8_t,
		 z1 = svsub_n_s8_x (p0, z1, d0),
		 z1 = svsub_x (p0, z1, d0))

/*
** sub_b0_s8_x_z2:
**	mov	(z[0-9]+\.b), b0
**	sub	z2\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (sub_b0_s8_x_z2, svint8_t, int8_t,
		 z2 = svsub_n_s8_x (p0, z1, d0),
		 z2 = svsub_x (p0, z1, d0))

/*
** sub_1_s8_x:
**	add	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (sub_1_s8_x, svint8_t,
		z0 = svsub_n_s8_x (p0, z0, 1),
		z0 = svsub_x (p0, z0, 1))

/*
** sub_1_s8_x_prfx:
**	movprfx	z0, z1
**	add	z0\.b, z0\.b, #255
**	ret
*/
TEST_UNIFORM_Z (sub_1_s8_x_prfx, svint8_t,
		z0 = svsub_n_s8_x (p0, z1, 1),
		z0 = svsub_x (p0, z1, 1))

/*
** sub_127_s8_x:
**	add	z0\.b, z0\.b, #129
**	ret
*/
TEST_UNIFORM_Z (sub_127_s8_x, svint8_t,
		z0 = svsub_n_s8_x (p0, z0, 127),
		z0 = svsub_x (p0, z0, 127))

/*
** sub_128_s8_x:
**	add	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (sub_128_s8_x, svint8_t,
		z0 = svsub_n_s8_x (p0, z0, 128),
		z0 = svsub_x (p0, z0, 128))

/*
** sub_255_s8_x:
**	add	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (sub_255_s8_x, svint8_t,
		z0 = svsub_n_s8_x (p0, z0, 255),
		z0 = svsub_x (p0, z0, 255))

/*
** sub_m1_s8_x:
**	add	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (sub_m1_s8_x, svint8_t,
		z0 = svsub_n_s8_x (p0, z0, -1),
		z0 = svsub_x (p0, z0, -1))

/*
** sub_m127_s8_x:
**	add	z0\.b, z0\.b, #127
**	ret
*/
TEST_UNIFORM_Z (sub_m127_s8_x, svint8_t,
		z0 = svsub_n_s8_x (p0, z0, -127),
		z0 = svsub_x (p0, z0, -127))

/*
** sub_m128_s8_x:
**	add	z0\.b, z0\.b, #128
**	ret
*/
TEST_UNIFORM_Z (sub_m128_s8_x, svint8_t,
		z0 = svsub_n_s8_x (p0, z0, -128),
		z0 = svsub_x (p0, z0, -128))
