/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** max_s64_m_tied1:
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_m_tied1, svint64_t,
		z0 = svmax_s64_m (p0, z0, z1),
		z0 = svmax_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (max_s64_m_tied2, svint64_t,
		z0 = svmax_s64_m (p0, z1, z0),
		z0 = svmax_m (p0, z1, z0))

/*
** max_s64_m_untied:
**	movprfx	z0, z1
**	smax	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_m_untied, svint64_t,
		z0 = svmax_s64_m (p0, z1, z2),
		z0 = svmax_m (p0, z1, z2))

/*
** max_x0_s64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_s64_m_tied1, svint64_t, int64_t,
		 z0 = svmax_n_s64_m (p0, z0, x0),
		 z0 = svmax_m (p0, z0, x0))

/*
** max_x0_s64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_s64_m_untied, svint64_t, int64_t,
		 z0 = svmax_n_s64_m (p0, z1, x0),
		 z0 = svmax_m (p0, z1, x0))

/*
** max_d4_s64_m_tied1:
**	mov	(z[0-9]+\.d), d4
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d4_s64_m_tied1, svint64_t, int64_t,
		 z0 = svmax_n_s64_m (p0, z0, d4),
		 z0 = svmax_m (p0, z0, d4))

/*
** max_d4_s64_m_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d4_s64_m_untied, svint64_t, int64_t,
		 z0 = svmax_n_s64_m (p0, z1, d4),
		 z0 = svmax_m (p0, z1, d4))

/*
** max_1_s64_m_tied1:
**	mov	(z[0-9]+\.d), #1
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_s64_m_tied1, svint64_t,
		z0 = svmax_n_s64_m (p0, z0, 1),
		z0 = svmax_m (p0, z0, 1))

/*
** max_1_s64_m_untied:
**	mov	(z[0-9]+\.d), #1
**	movprfx	z0, z1
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_s64_m_untied, svint64_t,
		z0 = svmax_n_s64_m (p0, z1, 1),
		z0 = svmax_m (p0, z1, 1))

/*
** max_m1_s64_m:
**	mov	(z[0-9]+)\.b, #(-1|255)
**	smax	z0\.d, p0/m, z0\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (max_m1_s64_m, svint64_t,
		z0 = svmax_n_s64_m (p0, z0, -1),
		z0 = svmax_m (p0, z0, -1))

/*
** max_s64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_z_tied1, svint64_t,
		z0 = svmax_s64_z (p0, z0, z1),
		z0 = svmax_z (p0, z0, z1))

/*
** max_s64_z_tied2:
**	movprfx	z0\.d, p0/z, z0\.d
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_z_tied2, svint64_t,
		z0 = svmax_s64_z (p0, z1, z0),
		z0 = svmax_z (p0, z1, z0))

/*
** max_s64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	smax	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_z_untied, svint64_t,
		z0 = svmax_s64_z (p0, z1, z2),
		z0 = svmax_z (p0, z1, z2))

/*
** max_x0_s64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_s64_z_tied1, svint64_t, int64_t,
		 z0 = svmax_n_s64_z (p0, z0, x0),
		 z0 = svmax_z (p0, z0, x0))

/*
** max_x0_s64_z_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_s64_z_untied, svint64_t, int64_t,
		 z0 = svmax_n_s64_z (p0, z1, x0),
		 z0 = svmax_z (p0, z1, x0))

/*
** max_d4_s64_z_tied1:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.d, p0/z, z0\.d
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d4_s64_z_tied1, svint64_t, int64_t,
		 z0 = svmax_n_s64_z (p0, z0, d4),
		 z0 = svmax_z (p0, z0, d4))

/*
** max_d4_s64_z_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.d, p0/z, z1\.d
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d4_s64_z_untied, svint64_t, int64_t,
		 z0 = svmax_n_s64_z (p0, z1, d4),
		 z0 = svmax_z (p0, z1, d4))

/*
** max_1_s64_z_tied1:
**	mov	(z[0-9]+\.d), #1
**	movprfx	z0\.d, p0/z, z0\.d
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_s64_z_tied1, svint64_t,
		z0 = svmax_n_s64_z (p0, z0, 1),
		z0 = svmax_z (p0, z0, 1))

/*
** max_1_s64_z_untied: { xfail *-*-* }
**	mov	(z[0-9]+\.d), #1
**	movprfx	z0\.d, p0/z, z1\.d
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_s64_z_untied, svint64_t,
		z0 = svmax_n_s64_z (p0, z1, 1),
		z0 = svmax_z (p0, z1, 1))

/*
** max_s64_x_tied1:
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_x_tied1, svint64_t,
		z0 = svmax_s64_x (p0, z0, z1),
		z0 = svmax_x (p0, z0, z1))

/*
** max_s64_x_tied2:
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_x_tied2, svint64_t,
		z0 = svmax_s64_x (p0, z1, z0),
		z0 = svmax_x (p0, z1, z0))

/*
** max_s64_x_untied:
**	movprfx	z0, z1
**	smax	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (max_s64_x_untied, svint64_t,
		z0 = svmax_s64_x (p0, z1, z2),
		z0 = svmax_x (p0, z1, z2))

/*
** max_x0_s64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_x0_s64_x_tied1, svint64_t, int64_t,
		 z0 = svmax_n_s64_x (p0, z0, x0),
		 z0 = svmax_x (p0, z0, x0))

/*
** max_x0_s64_x_untied:
**	mov	z0\.d, x0
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (max_x0_s64_x_untied, svint64_t, int64_t,
		 z0 = svmax_n_s64_x (p0, z1, x0),
		 z0 = svmax_x (p0, z1, x0))

/*
** max_d4_s64_x_tied1:
**	mov	(z[0-9]+\.d), d4
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (max_d4_s64_x_tied1, svint64_t, int64_t,
		 z0 = svmax_n_s64_x (p0, z0, d4),
		 z0 = svmax_x (p0, z0, d4))

/*
** max_d4_s64_x_untied:
**	mov	z0\.d, d4
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (max_d4_s64_x_untied, svint64_t, int64_t,
		 z0 = svmax_n_s64_x (p0, z1, d4),
		 z0 = svmax_x (p0, z1, d4))

/*
** max_1_s64_x_tied1:
**	mov	(z[0-9]+\.d), #1
**	smax	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (max_1_s64_x_tied1, svint64_t,
		z0 = svmax_n_s64_x (p0, z0, 1),
		z0 = svmax_x (p0, z0, 1))

/*
** max_1_s64_x_untied:
**	mov	z0\.d, #1
**	smax	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (max_1_s64_x_untied, svint64_t,
		z0 = svmax_n_s64_x (p0, z1, 1),
		z0 = svmax_x (p0, z1, 1))
