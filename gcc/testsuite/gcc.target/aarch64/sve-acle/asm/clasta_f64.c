/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** clasta_f64_tied1:
**	clasta	z0\.d, p0, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (clasta_f64_tied1, svfloat64_t,
		z0 = svclasta_f64 (p0, z0, z1),
		z0 = svclasta (p0, z0, z1))

/*
** clasta_f64_tied2:
**	mov	(z[0-9]+\.d), z0\.d
**	movprfx	z0, z1
**	clasta	z0\.d, p0, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (clasta_f64_tied2, svfloat64_t,
		z0 = svclasta_f64 (p0, z1, z0),
		z0 = svclasta (p0, z1, z0))

/*
** clasta_f64_untied:
**	movprfx	z0, z1
**	clasta	z0\.d, p0, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (clasta_f64_untied, svfloat64_t,
		z0 = svclasta_f64 (p0, z1, z2),
		z0 = svclasta (p0, z1, z2))

/*
** clasta_x0_x0_f64:
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x0_f64, float64_t, svfloat64_t,
		x0 = svclasta_n_f64 (p0, x0, z2),
		x0 = svclasta (p0, x0, z2))

/*
** clasta_x0_x1_f64:
**	mov	x0, x1
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x1_f64, float64_t, svfloat64_t,
		x0 = svclasta_n_f64 (p0, x1, z2),
		x0 = svclasta (p0, x1, z2))

/*
** clasta_x0_d0_f64:
**	fmov	x0, d0
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d0_f64, float64_t, svfloat64_t,
		x0 = svclasta_n_f64 (p0, d0, z2),
		x0 = svclasta (p0, d0, z2))

/*
** clasta_x0_d1_f64:
**	fmov	x0, d1
**	clasta	x0, p0, x0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d1_f64, float64_t, svfloat64_t,
		x0 = svclasta_n_f64 (p0, d1, z2),
		x0 = svclasta (p0, d1, z2))

/*
** clasta_d0_x0_f64:
**	fmov	d0, x0
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x0_f64, float64_t, svfloat64_t,
		d0 = svclasta_n_f64 (p0, x0, z2),
		d0 = svclasta (p0, x0, z2))

/*
** clasta_d0_x1_f64:
**	fmov	d0, x1
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x1_f64, float64_t, svfloat64_t,
		d0 = svclasta_n_f64 (p0, x1, z2),
		d0 = svclasta (p0, x1, z2))

/*
** clasta_d0_d0_f64:
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d0_f64, float64_t, svfloat64_t,
		d0 = svclasta_n_f64 (p0, d0, z2),
		d0 = svclasta (p0, d0, z2))

/*
** clasta_d0_d1_f64:
**	fmov	d0, d1
**	clasta	d0, p0, d0, z2\.d
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d1_f64, float64_t, svfloat64_t,
		d0 = svclasta_n_f64 (p0, d1, z2),
		d0 = svclasta (p0, d1, z2))
