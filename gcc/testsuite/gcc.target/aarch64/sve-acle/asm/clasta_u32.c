/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** clasta_x0_x0_u32:
**	clasta	w0, p0, w0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x0_u32, uint32_t, svuint32_t,
		x0 = svclasta_n_u32 (p0, x0, z2),
		x0 = svclasta (p0, x0, z2))

/*
** clasta_x0_x1_u32:
**	mov	w0, w1
**	clasta	w0, p0, w0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_x1_u32, uint32_t, svuint32_t,
		x0 = svclasta_n_u32 (p0, x1, z2),
		x0 = svclasta (p0, x1, z2))

/*
** clasta_x0_d0_u32:
**	fmov	w0, s0
**	clasta	w0, p0, w0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d0_u32, uint32_t, svuint32_t,
		x0 = svclasta_n_u32 (p0, d0, z2),
		x0 = svclasta (p0, d0, z2))

/*
** clasta_x0_d1_u32:
**	fmov	w0, s1
**	clasta	w0, p0, w0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_x0_d1_u32, uint32_t, svuint32_t,
		x0 = svclasta_n_u32 (p0, d1, z2),
		x0 = svclasta (p0, d1, z2))

/*
** clasta_d0_x0_u32:
**	fmov	s0, w0
**	clasta	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x0_u32, uint32_t, svuint32_t,
		d0 = svclasta_n_u32 (p0, x0, z2),
		d0 = svclasta (p0, x0, z2))

/*
** clasta_d0_x1_u32:
**	fmov	s0, w1
**	clasta	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_x1_u32, uint32_t, svuint32_t,
		d0 = svclasta_n_u32 (p0, x1, z2),
		d0 = svclasta (p0, x1, z2))

/*
** clasta_d0_d0_u32:
**	clasta	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d0_u32, uint32_t, svuint32_t,
		d0 = svclasta_n_u32 (p0, d0, z2),
		d0 = svclasta (p0, d0, z2))

/*
** clasta_d0_d1_u32:
**	fmov	s0, s1
**	clasta	s0, p0, s0, z2\.s
**	ret
*/
TEST_FOLD_LEFT (clasta_d0_d1_u32, uint32_t, svuint32_t,
		d0 = svclasta_n_u32 (p0, d1, z2),
		d0 = svclasta (p0, d1, z2))

/*
** clasta_u32_tied1:
**	clasta	z0\.s, p0, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (clasta_u32_tied1, svuint32_t,
		z0 = svclasta_u32 (p0, z0, z1),
		z0 = svclasta (p0, z0, z1))

/*
** clasta_u32_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	clasta	z0\.s, p0, z0\.s, \1\.s
**	ret
*/
TEST_UNIFORM_Z (clasta_u32_tied2, svuint32_t,
		z0 = svclasta_u32 (p0, z1, z0),
		z0 = svclasta (p0, z1, z0))

/*
** clasta_u32_untied:
**	movprfx	z0, z1
**	clasta	z0\.s, p0, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (clasta_u32_untied, svuint32_t,
		z0 = svclasta_u32 (p0, z1, z2),
		z0 = svclasta (p0, z1, z2))
