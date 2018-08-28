/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** min_s32_m_tied1:
**	smin	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_m_tied1, svint32_t,
		z0 = svmin_s32_m (p0, z0, z1),
		z0 = svmin_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (min_s32_m_tied2, svint32_t,
		z1 = svmin_s32_m (p0, z0, z1),
		z1 = svmin_m (p0, z0, z1))

/*
** min_s32_m_untied:
**	movprfx	z0, z1
**	smin	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_m_untied, svint32_t,
		z0 = svmin_s32_m (p0, z1, z2),
		z0 = svmin_m (p0, z1, z2))

/*
** min_w0_s32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s32_m_tied1, svint32_t, int32_t,
		 z0 = svmin_n_s32_m (p0, z0, x0),
		 z0 = svmin_m (p0, z0, x0))

/*
** min_w0_s32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s32_m_untied, svint32_t, int32_t,
		 z0 = svmin_n_s32_m (p0, z1, x0),
		 z0 = svmin_m (p0, z1, x0))

/*
** min_s0_s32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	smin	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_s0_s32_m_tied1, svint32_t, int32_t,
		 z1 = svmin_n_s32_m (p0, z1, d0),
		 z1 = svmin_m (p0, z1, d0))

/*
** min_s0_s32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	smin	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_s0_s32_m_untied, svint32_t, int32_t,
		 z1 = svmin_n_s32_m (p0, z2, d0),
		 z1 = svmin_m (p0, z2, d0))

/*
** min_1_s32_m_tied1:
**	mov	(z[0-9]+\.s), #1
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s32_m_tied1, svint32_t,
		z0 = svmin_n_s32_m (p0, z0, 1),
		z0 = svmin_m (p0, z0, 1))

/*
** min_1_s32_m_untied:
**	mov	(z[0-9]+\.s), #1
**	movprfx	z0, z1
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s32_m_untied, svint32_t,
		z0 = svmin_n_s32_m (p0, z1, 1),
		z0 = svmin_m (p0, z1, 1))

/*
** min_m1_s32_m:
**	mov	(z[0-9]+)\.b, #(-1|255)
**	smin	z0\.s, p0/m, z0\.s, \1\.s
**	ret
*/
TEST_UNIFORM_Z (min_m1_s32_m, svint32_t,
		z0 = svmin_n_s32_m (p0, z0, -1),
		z0 = svmin_m (p0, z0, -1))

/*
** min_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	smin	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_z_tied1, svint32_t,
		z0 = svmin_s32_z (p0, z0, z1),
		z0 = svmin_z (p0, z0, z1))

/*
** min_s32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	smin	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_z_tied2, svint32_t,
		z1 = svmin_s32_z (p0, z0, z1),
		z1 = svmin_z (p0, z0, z1))

/*
** min_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	smin	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_z_untied, svint32_t,
		z0 = svmin_s32_z (p0, z1, z2),
		z0 = svmin_z (p0, z1, z2))

/*
** min_w0_s32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s32_z_tied1, svint32_t, int32_t,
		 z0 = svmin_n_s32_z (p0, z0, x0),
		 z0 = svmin_z (p0, z0, x0))

/*
** min_w0_s32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z1\.s
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s32_z_untied, svint32_t, int32_t,
		 z0 = svmin_n_s32_z (p0, z1, x0),
		 z0 = svmin_z (p0, z1, x0))

/*
** min_s0_s32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	smin	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_s0_s32_z_tied1, svint32_t, int32_t,
		 z1 = svmin_n_s32_z (p0, z1, d0),
		 z1 = svmin_z (p0, z1, d0))

/*
** min_s0_s32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z2\.s
**	smin	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_s0_s32_z_untied, svint32_t, int32_t,
		 z1 = svmin_n_s32_z (p0, z2, d0),
		 z1 = svmin_z (p0, z2, d0))

/*
** min_1_s32_z_tied1:
**	mov	(z[0-9]+\.s), #1
**	movprfx	z0\.s, p0/z, z0\.s
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s32_z_tied1, svint32_t,
		z0 = svmin_n_s32_z (p0, z0, 1),
		z0 = svmin_z (p0, z0, 1))

/*
** min_1_s32_z_untied:
**	mov	(z[0-9]+\.s), #1
**	movprfx	z0\.s, p0/z, z1\.s
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s32_z_untied, svint32_t,
		z0 = svmin_n_s32_z (p0, z1, 1),
		z0 = svmin_z (p0, z1, 1))

/*
** min_s32_x_tied1:
**	smin	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_x_tied1, svint32_t,
		z0 = svmin_s32_x (p0, z0, z1),
		z0 = svmin_x (p0, z0, z1))

/*
** min_s32_x_tied2:
**	smin	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_x_tied2, svint32_t,
		z1 = svmin_s32_x (p0, z0, z1),
		z1 = svmin_x (p0, z0, z1))

/*
** min_s32_x_untied:
**	movprfx	z2, z0
**	smin	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (min_s32_x_untied, svint32_t,
		z2 = svmin_s32_x (p0, z0, z1),
		z2 = svmin_x (p0, z0, z1))

/*
** min_w0_s32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s32_x_tied1, svint32_t, int32_t,
		 z0 = svmin_n_s32_x (p0, z0, x0),
		 z0 = svmin_x (p0, z0, x0))

/*
** min_w0_s32_x_untied:
**	mov	z1\.s, w0
**	smin	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (min_w0_s32_x_untied, svint32_t, int32_t,
		 z1 = svmin_n_s32_x (p0, z0, x0),
		 z1 = svmin_x (p0, z0, x0))

/*
** min_s0_s32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	smin	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (min_s0_s32_x_tied1, svint32_t, int32_t,
		 z1 = svmin_n_s32_x (p0, z1, d0),
		 z1 = svmin_x (p0, z1, d0))

/*
** min_s0_s32_x_untied:
**	mov	z2\.s, s0
**	smin	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (min_s0_s32_x_untied, svint32_t, int32_t,
		 z2 = svmin_n_s32_x (p0, z1, d0),
		 z2 = svmin_x (p0, z1, d0))

/*
** min_1_s32_x_tied1:
**	mov	(z[0-9]+\.s), #1
**	smin	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s32_x_tied1, svint32_t,
		z0 = svmin_n_s32_x (p0, z0, 1),
		z0 = svmin_x (p0, z0, 1))

/*
** min_1_s32_x_untied:
**	mov	z0\.s, #1
**	smin	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (min_1_s32_x_untied, svint32_t,
		z0 = svmin_n_s32_x (p0, z1, 1),
		z0 = svmin_x (p0, z1, 1))

