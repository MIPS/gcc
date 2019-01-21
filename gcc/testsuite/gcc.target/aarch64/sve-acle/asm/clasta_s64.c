/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** clasta_x0_x0_s64:
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x0_s64, int64_t, svint64_t,
		x0 = svclasta_n_s64 (p0, x0, z2),
		x0 = svclasta (p0, x0, z2))

/*
** clasta_x0_x1_s64:
**	mov	x0, x1
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x1_s64, int64_t, svint64_t,
		x0 = svclasta_n_s64 (p0, x1, z2),
		x0 = svclasta (p0, x1, z2))

/*
** clasta_x0_d0_s64:
**	fmov	x0, d0
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d0_s64, int64_t, svint64_t,
		x0 = svclasta_n_s64 (p0, d0, z2),
		x0 = svclasta (p0, d0, z2))

/*
** clasta_x0_d1_s64:
**	fmov	x0, d1
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d1_s64, int64_t, svint64_t,
		x0 = svclasta_n_s64 (p0, d1, z2),
		x0 = svclasta (p0, d1, z2))

/*
** clasta_d0_x0_s64:
**	fmov	d0, x0
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x0_s64, int64_t, svint64_t,
		d0 = svclasta_n_s64 (p0, x0, z2),
		d0 = svclasta (p0, x0, z2))

/*
** clasta_d0_x1_s64:
**	fmov	d0, x1
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x1_s64, int64_t, svint64_t,
		d0 = svclasta_n_s64 (p0, x1, z2),
		d0 = svclasta (p0, x1, z2))

/*
** clasta_d0_d0_s64:
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d0_s64, int64_t, svint64_t,
		d0 = svclasta_n_s64 (p0, d0, z2),
		d0 = svclasta (p0, d0, z2))

/*
** clasta_d0_d1_s64:
**	fmov	d0, d1
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d1_s64, int64_t, svint64_t,
		d0 = svclasta_n_s64 (p0, d1, z2),
		d0 = svclasta (p0, d1, z2))

/*
** clasta_s64_tied1:
**	clasta	z0\.d, p0, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (clasta_s64_tied1, svint64_t,
		z0 = svclasta_s64 (p0, z0, z1),
		z0 = svclasta (p0, z0, z1))

/*
** clasta_s64_tied2:
**	mov	(z[0-9]+\.d), z0\.d
**	movprfx	z0, z1
**	clasta	z0\.d, p0, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (clasta_s64_tied2, svint64_t,
		z0 = svclasta_s64 (p0, z1, z0),
		z0 = svclasta (p0, z1, z0))

/*
** clasta_s64_untied:
**	movprfx	z0, z1
**	clasta	z0\.d, p0, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (clasta_s64_untied, svint64_t,
		z0 = svclasta_s64 (p0, z1, z2),
		z0 = svclasta (p0, z1, z2))
