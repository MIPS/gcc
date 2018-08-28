/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** min_s64_m_tied1:
**	smin	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_m_tied1, svint64_t,
		z0 = svmin_s64_m (p0, z0, z1),
		z0 = svmin_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (min_s64_m_tied2, svint64_t,
		z1 = svmin_s64_m (p0, z0, z1),
		z1 = svmin_m (p0, z0, z1))

/*
** min_s64_m_untied:
**	movprfx	z0, z1
**	smin	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_m_untied, svint64_t,
		z0 = svmin_s64_m (p0, z1, z2),
		z0 = svmin_m (p0, z1, z2))

/*
** min_x0_s64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_x0_s64_m_tied1, svint64_t, int64_t,
		 z0 = svmin_n_s64_m (p0, z0, x0),
		 z0 = svmin_m (p0, z0, x0))

/*
** min_x0_s64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_x0_s64_m_untied, svint64_t, int64_t,
		 z0 = svmin_n_s64_m (p0, z1, x0),
		 z0 = svmin_m (p0, z1, x0))

/*
** min_d0_s64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	smin	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_d0_s64_m_tied1, svint64_t, int64_t,
		 z1 = svmin_n_s64_m (p0, z1, d0),
		 z1 = svmin_m (p0, z1, d0))

/*
** min_d0_s64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	smin	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_d0_s64_m_untied, svint64_t, int64_t,
		 z1 = svmin_n_s64_m (p0, z2, d0),
		 z1 = svmin_m (p0, z2, d0))

/*
** min_1_s64_m_tied1:
**	mov	(z[0-9]+\.d), #1
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s64_m_tied1, svint64_t,
		z0 = svmin_n_s64_m (p0, z0, 1),
		z0 = svmin_m (p0, z0, 1))

/*
** min_1_s64_m_untied:
**	mov	(z[0-9]+\.d), #1
**	movprfx	z0, z1
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s64_m_untied, svint64_t,
		z0 = svmin_n_s64_m (p0, z1, 1),
		z0 = svmin_m (p0, z1, 1))

/*
** min_m1_s64_m:
**	mov	(z[0-9]+)\.b, #(-1|255)
**	smin	z0\.d, p0/m, z0\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (min_m1_s64_m, svint64_t,
		z0 = svmin_n_s64_m (p0, z0, -1),
		z0 = svmin_m (p0, z0, -1))

/*
** min_s64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	smin	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_z_tied1, svint64_t,
		z0 = svmin_s64_z (p0, z0, z1),
		z0 = svmin_z (p0, z0, z1))

/*
** min_s64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	smin	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_z_tied2, svint64_t,
		z1 = svmin_s64_z (p0, z0, z1),
		z1 = svmin_z (p0, z0, z1))

/*
** min_s64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	smin	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_z_untied, svint64_t,
		z0 = svmin_s64_z (p0, z1, z2),
		z0 = svmin_z (p0, z1, z2))

/*
** min_x0_s64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_x0_s64_z_tied1, svint64_t, int64_t,
		 z0 = svmin_n_s64_z (p0, z0, x0),
		 z0 = svmin_z (p0, z0, x0))

/*
** min_x0_s64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_x0_s64_z_untied, svint64_t, int64_t,
		 z0 = svmin_n_s64_z (p0, z1, x0),
		 z0 = svmin_z (p0, z1, x0))

/*
** min_d0_s64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	smin	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_d0_s64_z_tied1, svint64_t, int64_t,
		 z1 = svmin_n_s64_z (p0, z1, d0),
		 z1 = svmin_z (p0, z1, d0))

/*
** min_d0_s64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z2\.d
**	smin	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_d0_s64_z_untied, svint64_t, int64_t,
		 z1 = svmin_n_s64_z (p0, z2, d0),
		 z1 = svmin_z (p0, z2, d0))

/*
** min_1_s64_z_tied1:
**	mov	(z[0-9]+\.d), #1
**	movprfx	z0\.d, p0/z, z0\.d
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s64_z_tied1, svint64_t,
		z0 = svmin_n_s64_z (p0, z0, 1),
		z0 = svmin_z (p0, z0, 1))

/*
** min_1_s64_z_untied:
**	mov	(z[0-9]+\.d), #1
**	movprfx	z0\.d, p0/z, z1\.d
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s64_z_untied, svint64_t,
		z0 = svmin_n_s64_z (p0, z1, 1),
		z0 = svmin_z (p0, z1, 1))

/*
** min_s64_x_tied1:
**	smin	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_x_tied1, svint64_t,
		z0 = svmin_s64_x (p0, z0, z1),
		z0 = svmin_x (p0, z0, z1))

/*
** min_s64_x_tied2:
**	smin	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_x_tied2, svint64_t,
		z1 = svmin_s64_x (p0, z0, z1),
		z1 = svmin_x (p0, z0, z1))

/*
** min_s64_x_untied:
**	movprfx	z2, z0
**	smin	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (min_s64_x_untied, svint64_t,
		z2 = svmin_s64_x (p0, z0, z1),
		z2 = svmin_x (p0, z0, z1))

/*
** min_x0_s64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_x0_s64_x_tied1, svint64_t, int64_t,
		 z0 = svmin_n_s64_x (p0, z0, x0),
		 z0 = svmin_x (p0, z0, x0))

/*
** min_x0_s64_x_untied:
**	mov	z1\.d, x0
**	smin	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (min_x0_s64_x_untied, svint64_t, int64_t,
		 z1 = svmin_n_s64_x (p0, z0, x0),
		 z1 = svmin_x (p0, z0, x0))

/*
** min_d0_s64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	smin	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (min_d0_s64_x_tied1, svint64_t, int64_t,
		 z1 = svmin_n_s64_x (p0, z1, d0),
		 z1 = svmin_x (p0, z1, d0))

/*
** min_d0_s64_x_untied:
**	mov	z2\.d, d0
**	smin	z2\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (min_d0_s64_x_untied, svint64_t, int64_t,
		 z2 = svmin_n_s64_x (p0, z1, d0),
		 z2 = svmin_x (p0, z1, d0))

/*
** min_1_s64_x_tied1:
**	mov	(z[0-9]+\.d), #1
**	smin	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (min_1_s64_x_tied1, svint64_t,
		z0 = svmin_n_s64_x (p0, z0, 1),
		z0 = svmin_x (p0, z0, 1))

/*
** min_1_s64_x_untied:
**	mov	z0\.d, #1
**	smin	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (min_1_s64_x_untied, svint64_t,
		z0 = svmin_n_s64_x (p0, z1, 1),
		z0 = svmin_x (p0, z1, 1))

