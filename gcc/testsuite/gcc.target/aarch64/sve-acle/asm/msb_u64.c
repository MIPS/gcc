/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** msb_u64_m_tied1:
**	msb	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_m_tied1, svuint64_t,
		z0 = svmsb_u64_m (p0, z0, z1, z2),
		z0 = svmsb_m (p0, z0, z1, z2))

/*
** msb_u64_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	msb	z1\.d, p0/m, \1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_m_tied2, svuint64_t,
		z1 = svmsb_u64_m (p0, z0, z1, z2),
		z1 = svmsb_m (p0, z0, z1, z2))

/*
** msb_u64_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	msb	z2\.d, p0/m, z1\.d, \1\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_m_tied3, svuint64_t,
		z2 = svmsb_u64_m (p0, z0, z1, z2),
		z2 = svmsb_m (p0, z0, z1, z2))

/*
** msb_u64_m_untied:
**	movprfx	z0, z1
**	msb	z0\.d, p0/m, z2\.d, z3\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_m_untied, svuint64_t,
		z0 = svmsb_u64_m (p0, z1, z2, z3),
		z0 = svmsb_m (p0, z1, z2, z3))

/*
** msb_x0_u64_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_m_tied1, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_m (p0, z0, z1, x0),
		 z0 = svmsb_m (p0, z0, z1, x0))

/*
** msb_x0_u64_m_tied2:
**	mov	(z[0-9]+\.d), x0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	msb	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_m_tied2, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_m (p0, z0, z1, x0),
		 z1 = svmsb_m (p0, z0, z1, x0))

/*
** msb_x0_u64_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	msb	z0\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_m_untied, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_m (p0, z1, z2, x0),
		 z0 = svmsb_m (p0, z1, z2, x0))

/*
** msb_d0_u64_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_m_tied1, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_m (p0, z0, z1, d0),
		 z0 = svmsb_m (p0, z0, z1, d0))

/*
** msb_d0_u64_m_tied2:
**	mov	(z[0-9]+\.d), d0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	msb	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_m_tied2, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_m (p0, z0, z1, d0),
		 z1 = svmsb_m (p0, z0, z1, d0))

/*
** msb_d0_u64_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z0
**	msb	z1\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_m_untied, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_m (p0, z0, z2, d0),
		 z1 = svmsb_m (p0, z0, z2, d0))

/*
** msb_2_u64_m_tied1:
**	mov	(z[0-9]+\.d), #2
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_Z (msb_2_u64_m_tied1, svuint64_t,
		z0 = svmsb_n_u64_m (p0, z0, z1, 2),
		z0 = svmsb_m (p0, z0, z1, 2))

/*
** msb_2_u64_m_tied2:
**	mov	(z[0-9]+\.d), #2
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	msb	z1\.d, p0/m, \2\.d, \1
**	ret
*/
TEST_UNIFORM_Z (msb_2_u64_m_tied2, svuint64_t,
		z1 = svmsb_n_u64_m (p0, z0, z1, 2),
		z1 = svmsb_m (p0, z0, z1, 2))

/*
** msb_2_u64_m_untied:
**	mov	(z[0-9]+\.d), #2
**	movprfx	z0, z1
**	msb	z0\.d, p0/m, z2\.d, \1
**	ret
*/
TEST_UNIFORM_Z (msb_2_u64_m_untied, svuint64_t,
		z0 = svmsb_n_u64_m (p0, z1, z2, 2),
		z0 = svmsb_m (p0, z1, z2, 2))

/*
** msb_u64_z_tied1:
**	movprfx	z0\.d, p0/z, z0\.d
**	msb	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_z_tied1, svuint64_t,
		z0 = svmsb_u64_z (p0, z0, z1, z2),
		z0 = svmsb_z (p0, z0, z1, z2))

/*
** msb_u64_z_tied2:
**	movprfx	z1\.d, p0/z, z1\.d
**	msb	z1\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_z_tied2, svuint64_t,
		z1 = svmsb_u64_z (p0, z0, z1, z2),
		z1 = svmsb_z (p0, z0, z1, z2))

/*
** msb_u64_z_tied3:
**	movprfx	z2\.d, p0/z, z2\.d
**	mls	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_z_tied3, svuint64_t,
		z2 = svmsb_u64_z (p0, z0, z1, z2),
		z2 = svmsb_z (p0, z0, z1, z2))

/*
** msb_u64_z_untied:
**	movprfx	z0\.d, p0/z, z3\.d
**	mls	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_z_untied, svuint64_t,
		z0 = svmsb_u64_z (p0, z1, z2, z3),
		z0 = svmsb_z (p0, z1, z2, z3))

/*
** msb_x0_u64_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_z_tied1, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_z (p0, z0, z1, x0),
		 z0 = svmsb_z (p0, z0, z1, x0))

/*
** msb_x0_u64_z_tied2:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z1\.d, p0/z, z1\.d
**	msb	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_z_tied2, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_z (p0, z0, z1, x0),
		 z1 = svmsb_z (p0, z0, z1, x0))

/*
** msb_x0_u64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, \1
**	mls	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_z_untied, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_z (p0, z1, z2, x0),
		 z0 = svmsb_z (p0, z1, z2, x0))

/*
** msb_d0_u64_z_tied1:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z0\.d, p0/z, z0\.d
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_z_tied1, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_z (p0, z0, z1, d0),
		 z0 = svmsb_z (p0, z0, z1, d0))

/*
** msb_d0_u64_z_tied2:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	msb	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_z_tied2, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_z (p0, z0, z1, d0),
		 z1 = svmsb_z (p0, z0, z1, d0))

/*
** msb_d0_u64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, \1
**	mls	z1\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_z_untied, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_z (p0, z0, z2, d0),
		 z1 = svmsb_z (p0, z0, z2, d0))

/*
** msb_u64_x_tied1:
**	msb	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_x_tied1, svuint64_t,
		z0 = svmsb_u64_x (p0, z0, z1, z2),
		z0 = svmsb_x (p0, z0, z1, z2))

/*
** msb_u64_x_tied2:
**	msb	z1\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_x_tied2, svuint64_t,
		z1 = svmsb_u64_x (p0, z0, z1, z2),
		z1 = svmsb_x (p0, z0, z1, z2))

/*
** msb_u64_x_tied3:
**	mls	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_x_tied3, svuint64_t,
		z2 = svmsb_u64_x (p0, z0, z1, z2),
		z2 = svmsb_x (p0, z0, z1, z2))

/*
** msb_u64_x_untied:
**	movprfx	z3, z2
**	mls	z3\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (msb_u64_x_untied, svuint64_t,
		z3 = svmsb_u64_x (p0, z0, z1, z2),
		z3 = svmsb_x (p0, z0, z1, z2))

/*
** msb_x0_u64_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_x_tied1, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_x (p0, z0, z1, x0),
		 z0 = svmsb_x (p0, z0, z1, x0))

/*
** msb_x0_u64_x_tied2:
**	mov	(z[0-9]+\.d), x0
**	msb	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_x_tied2, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_x (p0, z0, z1, x0),
		 z1 = svmsb_x (p0, z0, z1, x0))

/*
** msb_x0_u64_x_untied:
**	mov	z2\.d, x0
**	mls	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (msb_x0_u64_x_untied, svuint64_t, uint64_t,
		 z2 = svmsb_n_u64_x (p0, z0, z1, x0),
		 z2 = svmsb_x (p0, z0, z1, x0))

/*
** msb_d0_u64_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_x_tied1, svuint64_t, uint64_t,
		 z0 = svmsb_n_u64_x (p0, z0, z1, d0),
		 z0 = svmsb_x (p0, z0, z1, d0))

/*
** msb_d0_u64_x_tied2:
**	mov	(z[0-9]+\.d), d0
**	msb	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_x_tied2, svuint64_t, uint64_t,
		 z1 = svmsb_n_u64_x (p0, z0, z1, d0),
		 z1 = svmsb_x (p0, z0, z1, d0))

/*
** msb_d0_u64_x_untied:
**	mov	z2\.d, d0
**	mls	z2\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_ZS (msb_d0_u64_x_untied, svuint64_t, uint64_t,
		 z2 = svmsb_n_u64_x (p0, z0, z1, d0),
		 z2 = svmsb_x (p0, z0, z1, d0))

/*
** msb_2_u64_x_tied1:
**	mov	(z[0-9]+\.d), #2
**	msb	z0\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_Z (msb_2_u64_x_tied1, svuint64_t,
		z0 = svmsb_n_u64_x (p0, z0, z1, 2),
		z0 = svmsb_x (p0, z0, z1, 2))

/*
** msb_2_u64_x_tied2:
**	mov	(z[0-9]+\.d), #2
**	msb	z1\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (msb_2_u64_x_tied2, svuint64_t,
		z1 = svmsb_n_u64_x (p0, z0, z1, 2),
		z1 = svmsb_x (p0, z0, z1, 2))

/*
** msb_2_u64_x_untied:
**	mov	z0\.d, #2
**	mls	z0\.d, p0/m, z1\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (msb_2_u64_x_untied, svuint64_t,
		z0 = svmsb_n_u64_x (p0, z1, z2, 2),
		z0 = svmsb_x (p0, z1, z2, 2))

