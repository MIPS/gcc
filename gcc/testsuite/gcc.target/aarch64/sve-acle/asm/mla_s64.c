/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mla_s64_m_tied1:
**	mla	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_m_tied1, svint64_t,
		z0 = svmla_s64_m (p0, z0, z1, z2),
		z0 = svmla_m (p0, z0, z1, z2))

/*
** mla_s64_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mla	z1\.d, p0/m, \1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_m_tied2, svint64_t,
		z1 = svmla_s64_m (p0, z0, z1, z2),
		z1 = svmla_m (p0, z0, z1, z2))

/*
** mla_s64_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	mla	z2\.d, p0/m, z1\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_m_tied3, svint64_t,
		z2 = svmla_s64_m (p0, z0, z1, z2),
		z2 = svmla_m (p0, z0, z1, z2))

/*
** mla_s64_m_untied:
**	movprfx	z0, z1
**	mla	z0\.d, p0/m, z2\.d, z3\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_m_untied, svint64_t,
		z0 = svmla_s64_m (p0, z1, z2, z3),
		z0 = svmla_m (p0, z1, z2, z3))

/*
** mla_x0_s64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_m_tied1, svint64_t, int64_t,
		 z0 = svmla_n_s64_m (p0, z0, z1, x0),
		 z0 = svmla_m (p0, z0, z1, x0))

/*
** mla_x0_s64_m_tied2:
**	mov	(z[0-9]+\.d), x0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mla	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_m_tied2, svint64_t, int64_t,
		 z1 = svmla_n_s64_m (p0, z0, z1, x0),
		 z1 = svmla_m (p0, z0, z1, x0))

/*
** mla_x0_s64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	mla	z0\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_m_untied, svint64_t, int64_t,
		 z0 = svmla_n_s64_m (p0, z1, z2, x0),
		 z0 = svmla_m (p0, z1, z2, x0))

/*
** mla_d0_s64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_m_tied1, svint64_t, int64_t,
		 z0 = svmla_n_s64_m (p0, z0, z1, d0),
		 z0 = svmla_m (p0, z0, z1, d0))

/*
** mla_d0_s64_m_tied2:
**	mov	(z[0-9]+\.d), d0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mla	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_m_tied2, svint64_t, int64_t,
		 z1 = svmla_n_s64_m (p0, z0, z1, d0),
		 z1 = svmla_m (p0, z0, z1, d0))

/*
** mla_d0_s64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z0
**	mla	z1\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_m_untied, svint64_t, int64_t,
		 z1 = svmla_n_s64_m (p0, z0, z2, d0),
		 z1 = svmla_m (p0, z0, z2, d0))

/*
** mla_2_s64_m_tied1:
**	mov	(z[0-9]+\.d), #2
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mla_2_s64_m_tied1, svint64_t,
		z0 = svmla_n_s64_m (p0, z0, z1, 2),
		z0 = svmla_m (p0, z0, z1, 2))

/*
** mla_2_s64_m_tied2:
**	mov	(z[0-9]+\.d), #2
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mla	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mla_2_s64_m_tied2, svint64_t,
		z1 = svmla_n_s64_m (p0, z0, z1, 2),
		z1 = svmla_m (p0, z0, z1, 2))

/*
** mla_2_s64_m_untied:
**	mov	(z[0-9]+\.d), #2
**	movprfx	z0, z1
**	mla	z0\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mla_2_s64_m_untied, svint64_t,
		z0 = svmla_n_s64_m (p0, z1, z2, 2),
		z0 = svmla_m (p0, z1, z2, 2))

/*
** mla_s64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	mla	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_z_tied1, svint64_t,
		z0 = svmla_s64_z (p0, z0, z1, z2),
		z0 = svmla_z (p0, z0, z1, z2))

/*
** mla_s64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	mad	z1\.d, p0/m, z2\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_z_tied2, svint64_t,
		z1 = svmla_s64_z (p0, z0, z1, z2),
		z1 = svmla_z (p0, z0, z1, z2))

/*
** mla_s64_z_tied3:
**	movprfx	z2\.d, p0/z, z2\.d
**	mad	z2\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_z_tied3, svint64_t,
		z2 = svmla_s64_z (p0, z0, z1, z2),
		z2 = svmla_z (p0, z0, z1, z2))

/*
** mla_s64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	mla	z0\.d, p0/m, z2\.d, z3\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_z_untied, svint64_t,
		z0 = svmla_s64_z (p0, z1, z2, z3),
		z0 = svmla_z (p0, z1, z2, z3))

/*
** mla_x0_s64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_z_tied1, svint64_t, int64_t,
		 z0 = svmla_n_s64_z (p0, z0, z1, x0),
		 z0 = svmla_z (p0, z0, z1, x0))

/*
** mla_x0_s64_z_tied2:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z1\.d, p0/z, z1\.d
**	mad	z1\.d, p0/m, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_z_tied2, svint64_t, int64_t,
		 z1 = svmla_n_s64_z (p0, z0, z1, x0),
		 z1 = svmla_z (p0, z0, z1, x0))

/*
** mla_x0_s64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	mla	z0\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_z_untied, svint64_t, int64_t,
		 z0 = svmla_n_s64_z (p0, z1, z2, x0),
		 z0 = svmla_z (p0, z1, z2, x0))

/*
** mla_d0_s64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z0\.d, p0/z, z0\.d
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_z_tied1, svint64_t, int64_t,
		 z0 = svmla_n_s64_z (p0, z0, z1, d0),
		 z0 = svmla_z (p0, z0, z1, d0))

/*
** mla_d0_s64_z_tied2:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	mad	z1\.d, p0/m, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_z_tied2, svint64_t, int64_t,
		 z1 = svmla_n_s64_z (p0, z0, z1, d0),
		 z1 = svmla_z (p0, z0, z1, d0))

/*
** mla_d0_s64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z0\.d
**	mla	z1\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_z_untied, svint64_t, int64_t,
		 z1 = svmla_n_s64_z (p0, z0, z2, d0),
		 z1 = svmla_z (p0, z0, z2, d0))

/*
** mla_s64_x_tied1:
**	mla	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_x_tied1, svint64_t,
		z0 = svmla_s64_x (p0, z0, z1, z2),
		z0 = svmla_x (p0, z0, z1, z2))

/*
** mla_s64_x_tied2:
**	mad	z1\.d, p0/m, z2\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_x_tied2, svint64_t,
		z1 = svmla_s64_x (p0, z0, z1, z2),
		z1 = svmla_x (p0, z0, z1, z2))

/*
** mla_s64_x_tied3:
**	mad	z2\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_x_tied3, svint64_t,
		z2 = svmla_s64_x (p0, z0, z1, z2),
		z2 = svmla_x (p0, z0, z1, z2))

/*
** mla_s64_x_untied:
**	movprfx	z3, z0
**	mla	z3\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (mla_s64_x_untied, svint64_t,
		z3 = svmla_s64_x (p0, z0, z1, z2),
		z3 = svmla_x (p0, z0, z1, z2))

/*
** mla_x0_s64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_x_tied1, svint64_t, int64_t,
		 z0 = svmla_n_s64_x (p0, z0, z1, x0),
		 z0 = svmla_x (p0, z0, z1, x0))

/*
** mla_x0_s64_x_tied2:
**	mov	(z[0-9]+\.d), x0
**	mad	z1\.d, p0/m, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_x_tied2, svint64_t, int64_t,
		 z1 = svmla_n_s64_x (p0, z0, z1, x0),
		 z1 = svmla_x (p0, z0, z1, x0))

/*
** mla_x0_s64_x_untied:
**	mov	z2\.d, x0
**	mad	z2\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mla_x0_s64_x_untied, svint64_t, int64_t,
		 z2 = svmla_n_s64_x (p0, z0, z1, x0),
		 z2 = svmla_x (p0, z0, z1, x0))

/*
** mla_d0_s64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_x_tied1, svint64_t, int64_t,
		 z0 = svmla_n_s64_x (p0, z0, z1, d0),
		 z0 = svmla_x (p0, z0, z1, d0))

/*
** mla_d0_s64_x_tied2:
**	mov	(z[0-9]+\.d), d0
**	mad	z1\.d, p0/m, \1, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_x_tied2, svint64_t, int64_t,
		 z1 = svmla_n_s64_x (p0, z0, z1, d0),
		 z1 = svmla_x (p0, z0, z1, d0))

/*
** mla_d0_s64_x_untied:
**	mov	z2\.d, d0
**	mad	z2\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_ZS (mla_d0_s64_x_untied, svint64_t, int64_t,
		 z2 = svmla_n_s64_x (p0, z0, z1, d0),
		 z2 = svmla_x (p0, z0, z1, d0))

/*
** mla_2_s64_x_tied1:
**	mov	(z[0-9]+\.d), #2
**	mla	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_Z (mla_2_s64_x_tied1, svint64_t,
		z0 = svmla_n_s64_x (p0, z0, z1, 2),
		z0 = svmla_x (p0, z0, z1, 2))

/*
** mla_2_s64_x_tied2:
**	mov	(z[0-9]+\.d), #2
**	mad	z1\.d, p0/m, \1, z0\.d
**	ret
*/
TEST_UNIFORM_Z (mla_2_s64_x_tied2, svint64_t,
		z1 = svmla_n_s64_x (p0, z0, z1, 2),
		z1 = svmla_x (p0, z0, z1, 2))

/*
** mla_2_s64_x_untied:
**	mov	z0\.d, #2
**	mad	z0\.d, p0/m, z2\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (mla_2_s64_x_untied, svint64_t,
		z0 = svmla_n_s64_x (p0, z1, z2, 2),
		z0 = svmla_x (p0, z1, z2, 2))

