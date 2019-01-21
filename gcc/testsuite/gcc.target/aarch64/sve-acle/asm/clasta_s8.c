/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** clasta_x0_x0_s8:
**	clasta	w0, p0, w0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x0_s8, int8_t, svint8_t,
		x0 = svclasta_n_s8 (p0, x0, z2),
		x0 = svclasta (p0, x0, z2))

/*
** clasta_x0_x1_s8:
**	mov	w0, w1
**	clasta	w0, p0, w0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x1_s8, int8_t, svint8_t,
		x0 = svclasta_n_s8 (p0, x1, z2),
		x0 = svclasta (p0, x1, z2))

/*
** clasta_x0_d0_s8:
**	umov	w0, v0\.b\[0\]
**	clasta	w0, p0, w0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d0_s8, int8_t, svint8_t,
		x0 = svclasta_n_s8 (p0, d0, z2),
		x0 = svclasta (p0, d0, z2))

/*
** clasta_x0_d1_s8:
**	umov	w0, v1\.b\[0\]
**	clasta	w0, p0, w0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d1_s8, int8_t, svint8_t,
		x0 = svclasta_n_s8 (p0, d1, z2),
		x0 = svclasta (p0, d1, z2))

/*
** clasta_d0_x0_s8:
**	dup	v0\.8b, w0
**	clasta	b0, p0, b0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x0_s8, int8_t, svint8_t,
		d0 = svclasta_n_s8 (p0, x0, z2),
		d0 = svclasta (p0, x0, z2))

/*
** clasta_d0_x1_s8:
**	dup	v0\.8b, w1
**	clasta	b0, p0, b0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x1_s8, int8_t, svint8_t,
		d0 = svclasta_n_s8 (p0, x1, z2),
		d0 = svclasta (p0, x1, z2))

/*
** clasta_d0_d0_s8:
**	clasta	b0, p0, b0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d0_s8, int8_t, svint8_t,
		d0 = svclasta_n_s8 (p0, d0, z2),
		d0 = svclasta (p0, d0, z2))

/*
** clasta_d0_d1_s8:
**	dup	b0, v1\.b\[0\]
**	clasta	b0, p0, b0, z2\.b
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d1_s8, int8_t, svint8_t,
		d0 = svclasta_n_s8 (p0, d1, z2),
		d0 = svclasta (p0, d1, z2))

/*
** clasta_s8_tied1:
**	clasta	z0\.b, p0, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (clasta_s8_tied1, svint8_t,
		z0 = svclasta_s8 (p0, z0, z1),
		z0 = svclasta (p0, z0, z1))

/*
** clasta_s8_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	clasta	z0\.b, p0, z0\.b, \1\.b
**	ret
*/
TEST_UNIFORM_Z (clasta_s8_tied2, svint8_t,
		z0 = svclasta_s8 (p0, z1, z0),
		z0 = svclasta (p0, z1, z0))

/*
** clasta_s8_untied:
**	movprfx	z0, z1
**	clasta	z0\.b, p0, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (clasta_s8_untied, svint8_t,
		z0 = svclasta_s8 (p0, z1, z2),
		z0 = svclasta (p0, z1, z2))
