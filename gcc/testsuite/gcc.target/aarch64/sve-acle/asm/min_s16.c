/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** min_s16_m_tied1:
**	smin	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_m_tied1, svint16_t,
		z0 = svmin_s16_m (p0, z0, z1),
		z0 = svmin_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (min_s16_m_tied2, svint16_t,
		z1 = svmin_s16_m (p0, z0, z1),
		z1 = svmin_m (p0, z0, z1))

/*
** min_s16_m_untied:
**	movprfx	z0, z1
**	smin	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_m_untied, svint16_t,
		z0 = svmin_s16_m (p0, z1, z2),
		z0 = svmin_m (p0, z1, z2))

/*
** min_w0_s16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s16_m_tied1, svint16_t, int16_t,
		 z0 = svmin_n_s16_m (p0, z0, x0),
		 z0 = svmin_m (p0, z0, x0))

/*
** min_w0_s16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s16_m_untied, svint16_t, int16_t,
		 z0 = svmin_n_s16_m (p0, z1, x0),
		 z0 = svmin_m (p0, z1, x0))

/*
** min_h0_s16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	smin	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_h0_s16_m_tied1, svint16_t, int16_t,
		 z1 = svmin_n_s16_m (p0, z1, d0),
		 z1 = svmin_m (p0, z1, d0))

/*
** min_h0_s16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z2
**	smin	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_h0_s16_m_untied, svint16_t, int16_t,
		 z1 = svmin_n_s16_m (p0, z2, d0),
		 z1 = svmin_m (p0, z2, d0))

/*
** min_1_s16_m_tied1:
**	mov	(z[0-9]+\.h), #1
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s16_m_tied1, svint16_t,
		z0 = svmin_n_s16_m (p0, z0, 1),
		z0 = svmin_m (p0, z0, 1))

/*
** min_1_s16_m_untied:
**	mov	(z[0-9]+\.h), #1
**	movprfx	z0, z1
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s16_m_untied, svint16_t,
		z0 = svmin_n_s16_m (p0, z1, 1),
		z0 = svmin_m (p0, z1, 1))

/*
** min_m1_s16_m:
**	mov	(z[0-9]+)\.b, #(-1|255)
**	smin	z0\.h, p0/m, z0\.h, \1\.h
**	ret
*/
TEST_UNIFORM_Z (min_m1_s16_m, svint16_t,
		z0 = svmin_n_s16_m (p0, z0, -1),
		z0 = svmin_m (p0, z0, -1))

/*
** min_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	smin	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_z_tied1, svint16_t,
		z0 = svmin_s16_z (p0, z0, z1),
		z0 = svmin_z (p0, z0, z1))

/*
** min_s16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	smin	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_z_tied2, svint16_t,
		z1 = svmin_s16_z (p0, z0, z1),
		z1 = svmin_z (p0, z0, z1))

/*
** min_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	smin	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_z_untied, svint16_t,
		z0 = svmin_s16_z (p0, z1, z2),
		z0 = svmin_z (p0, z1, z2))

/*
** min_w0_s16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s16_z_tied1, svint16_t, int16_t,
		 z0 = svmin_n_s16_z (p0, z0, x0),
		 z0 = svmin_z (p0, z0, x0))

/*
** min_w0_s16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z1\.h
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s16_z_untied, svint16_t, int16_t,
		 z0 = svmin_n_s16_z (p0, z1, x0),
		 z0 = svmin_z (p0, z1, x0))

/*
** min_h0_s16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	smin	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_h0_s16_z_tied1, svint16_t, int16_t,
		 z1 = svmin_n_s16_z (p0, z1, d0),
		 z1 = svmin_z (p0, z1, d0))

/*
** min_h0_s16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z2\.h
**	smin	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_h0_s16_z_untied, svint16_t, int16_t,
		 z1 = svmin_n_s16_z (p0, z2, d0),
		 z1 = svmin_z (p0, z2, d0))

/*
** min_1_s16_z_tied1:
**	mov	(z[0-9]+\.h), #1
**	movprfx	z0\.h, p0/z, z0\.h
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s16_z_tied1, svint16_t,
		z0 = svmin_n_s16_z (p0, z0, 1),
		z0 = svmin_z (p0, z0, 1))

/*
** min_1_s16_z_untied:
**	mov	(z[0-9]+\.h), #1
**	movprfx	z0\.h, p0/z, z1\.h
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s16_z_untied, svint16_t,
		z0 = svmin_n_s16_z (p0, z1, 1),
		z0 = svmin_z (p0, z1, 1))

/*
** min_s16_x_tied1:
**	smin	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_x_tied1, svint16_t,
		z0 = svmin_s16_x (p0, z0, z1),
		z0 = svmin_x (p0, z0, z1))

/*
** min_s16_x_tied2:
**	smin	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_x_tied2, svint16_t,
		z1 = svmin_s16_x (p0, z0, z1),
		z1 = svmin_x (p0, z0, z1))

/*
** min_s16_x_untied:
**	movprfx	z2, z0
**	smin	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (min_s16_x_untied, svint16_t,
		z2 = svmin_s16_x (p0, z0, z1),
		z2 = svmin_x (p0, z0, z1))

/*
** min_w0_s16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s16_x_tied1, svint16_t, int16_t,
		 z0 = svmin_n_s16_x (p0, z0, x0),
		 z0 = svmin_x (p0, z0, x0))

/*
** min_w0_s16_x_untied:
**	mov	z1\.h, w0
**	smin	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s16_x_untied, svint16_t, int16_t,
		 z1 = svmin_n_s16_x (p0, z0, x0),
		 z1 = svmin_x (p0, z0, x0))

/*
** min_h0_s16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	smin	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (min_h0_s16_x_tied1, svint16_t, int16_t,
		 z1 = svmin_n_s16_x (p0, z1, d0),
		 z1 = svmin_x (p0, z1, d0))

/*
** min_h0_s16_x_untied:
**	mov	z2\.h, h0
**	smin	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (min_h0_s16_x_untied, svint16_t, int16_t,
		 z2 = svmin_n_s16_x (p0, z1, d0),
		 z2 = svmin_x (p0, z1, d0))

/*
** min_1_s16_x_tied1:
**	mov	(z[0-9]+\.h), #1
**	smin	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s16_x_tied1, svint16_t,
		z0 = svmin_n_s16_x (p0, z0, 1),
		z0 = svmin_x (p0, z0, 1))

/*
** min_1_s16_x_untied:
**	mov	z0\.h, #1
**	smin	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (min_1_s16_x_untied, svint16_t,
		z0 = svmin_n_s16_x (p0, z1, 1),
		z0 = svmin_x (p0, z1, 1))

