/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mad_u16_m_tied1:
**	mad	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_m_tied1, svuint16_t,
		z0 = svmad_u16_m (p0, z0, z1, z2),
		z0 = svmad_m (p0, z0, z1, z2))

/*
** mad_u16_m_tied2:
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mad	z1\.h, p0/m, \1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_m_tied2, svuint16_t,
		z1 = svmad_u16_m (p0, z0, z1, z2),
		z1 = svmad_m (p0, z0, z1, z2))

/*
** mad_u16_m_tied3:
**	mov	(z[0-9]+)\.d, z2\.d
**	movprfx	z2, z0
**	mad	z2\.h, p0/m, z1\.h, \1\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_m_tied3, svuint16_t,
		z2 = svmad_u16_m (p0, z0, z1, z2),
		z2 = svmad_m (p0, z0, z1, z2))

/*
** mad_u16_m_untied:
**	movprfx	z0, z1
**	mad	z0\.h, p0/m, z2\.h, z3\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_m_untied, svuint16_t,
		z0 = svmad_u16_m (p0, z1, z2, z3),
		z0 = svmad_m (p0, z1, z2, z3))

/*
** mad_w0_u16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_m_tied1, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_m (p0, z0, z1, x0),
		 z0 = svmad_m (p0, z0, z1, x0))

/*
** mad_w0_u16_m_tied2:
**	mov	(z[0-9]+\.h), w0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mad	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_m_tied2, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_m (p0, z0, z1, x0),
		 z1 = svmad_m (p0, z0, z1, x0))

/*
** mad_w0_u16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	mad	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_m_untied, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_m (p0, z1, z2, x0),
		 z0 = svmad_m (p0, z1, z2, x0))

/*
** mad_h0_u16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_m_tied1, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_m (p0, z0, z1, d0),
		 z0 = svmad_m (p0, z0, z1, d0))

/*
** mad_h0_u16_m_tied2:
**	mov	(z[0-9]+\.h), h0
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mad	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_m_tied2, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_m (p0, z0, z1, d0),
		 z1 = svmad_m (p0, z0, z1, d0))

/*
** mad_h0_u16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z0
**	mad	z1\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_m_untied, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_m (p0, z0, z2, d0),
		 z1 = svmad_m (p0, z0, z2, d0))

/*
** mad_2_u16_m_tied1:
**	mov	(z[0-9]+\.h), #2
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_u16_m_tied1, svuint16_t,
		z0 = svmad_n_u16_m (p0, z0, z1, 2),
		z0 = svmad_m (p0, z0, z1, 2))

/*
** mad_2_u16_m_tied2:
**	mov	(z[0-9]+\.h), #2
**	mov	(z[0-9]+)\.d, z1\.d
**	movprfx	z1, z0
**	mad	z1\.h, p0/m, \2\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_u16_m_tied2, svuint16_t,
		z1 = svmad_n_u16_m (p0, z0, z1, 2),
		z1 = svmad_m (p0, z0, z1, 2))

/*
** mad_2_u16_m_untied:
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0, z1
**	mad	z0\.h, p0/m, z2\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_u16_m_untied, svuint16_t,
		z0 = svmad_n_u16_m (p0, z1, z2, 2),
		z0 = svmad_m (p0, z1, z2, 2))

/*
** mad_u16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	mad	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_z_tied1, svuint16_t,
		z0 = svmad_u16_z (p0, z0, z1, z2),
		z0 = svmad_z (p0, z0, z1, z2))

/*
** mad_u16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	mad	z1\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_z_tied2, svuint16_t,
		z1 = svmad_u16_z (p0, z0, z1, z2),
		z1 = svmad_z (p0, z0, z1, z2))

/*
** mad_u16_z_tied3:
**	movprfx	z2\.h, p0/z, z2\.h
**	mla	z2\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_z_tied3, svuint16_t,
		z2 = svmad_u16_z (p0, z0, z1, z2),
		z2 = svmad_z (p0, z0, z1, z2))

/*
** mad_u16_z_untied:
**	movprfx	z0\.h, p0/z, z3\.h
**	mla	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_z_untied, svuint16_t,
		z0 = svmad_u16_z (p0, z1, z2, z3),
		z0 = svmad_z (p0, z1, z2, z3))

/*
** mad_w0_u16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_z_tied1, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_z (p0, z0, z1, x0),
		 z0 = svmad_z (p0, z0, z1, x0))

/*
** mad_w0_u16_z_tied2:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z1\.h, p0/z, z1\.h
**	mad	z1\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_z_tied2, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_z (p0, z0, z1, x0),
		 z1 = svmad_z (p0, z0, z1, x0))

/*
** mad_w0_u16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, \1
**	mla	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_z_untied, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_z (p0, z1, z2, x0),
		 z0 = svmad_z (p0, z1, z2, x0))

/*
** mad_h0_u16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z0\.h, p0/z, z0\.h
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_z_tied1, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_z (p0, z0, z1, d0),
		 z0 = svmad_z (p0, z0, z1, d0))

/*
** mad_h0_u16_z_tied2:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	mad	z1\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_z_tied2, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_z (p0, z0, z1, d0),
		 z1 = svmad_z (p0, z0, z1, d0))

/*
** mad_h0_u16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, \1
**	mla	z1\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_z_untied, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_z (p0, z0, z2, d0),
		 z1 = svmad_z (p0, z0, z2, d0))

/*
** mad_u16_x_tied1:
**	mad	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_x_tied1, svuint16_t,
		z0 = svmad_u16_x (p0, z0, z1, z2),
		z0 = svmad_x (p0, z0, z1, z2))

/*
** mad_u16_x_tied2:
**	mad	z1\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_x_tied2, svuint16_t,
		z1 = svmad_u16_x (p0, z0, z1, z2),
		z1 = svmad_x (p0, z0, z1, z2))

/*
** mad_u16_x_tied3:
**	mla	z2\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_x_tied3, svuint16_t,
		z2 = svmad_u16_x (p0, z0, z1, z2),
		z2 = svmad_x (p0, z0, z1, z2))

/*
** mad_u16_x_untied:
**	movprfx	z3, z2
**	mla	z3\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mad_u16_x_untied, svuint16_t,
		z3 = svmad_u16_x (p0, z0, z1, z2),
		z3 = svmad_x (p0, z0, z1, z2))

/*
** mad_w0_u16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_x_tied1, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_x (p0, z0, z1, x0),
		 z0 = svmad_x (p0, z0, z1, x0))

/*
** mad_w0_u16_x_tied2:
**	mov	(z[0-9]+\.h), w0
**	mad	z1\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_x_tied2, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_x (p0, z0, z1, x0),
		 z1 = svmad_x (p0, z0, z1, x0))

/*
** mad_w0_u16_x_untied:
**	mov	z2\.h, w0
**	mla	z2\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (mad_w0_u16_x_untied, svuint16_t, uint16_t,
		 z2 = svmad_n_u16_x (p0, z0, z1, x0),
		 z2 = svmad_x (p0, z0, z1, x0))

/*
** mad_h0_u16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_x_tied1, svuint16_t, uint16_t,
		 z0 = svmad_n_u16_x (p0, z0, z1, d0),
		 z0 = svmad_x (p0, z0, z1, d0))

/*
** mad_h0_u16_x_tied2:
**	mov	(z[0-9]+\.h), h0
**	mad	z1\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_x_tied2, svuint16_t, uint16_t,
		 z1 = svmad_n_u16_x (p0, z0, z1, d0),
		 z1 = svmad_x (p0, z0, z1, d0))

/*
** mad_h0_u16_x_untied:
**	mov	z2\.h, h0
**	mla	z2\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (mad_h0_u16_x_untied, svuint16_t, uint16_t,
		 z2 = svmad_n_u16_x (p0, z0, z1, d0),
		 z2 = svmad_x (p0, z0, z1, d0))

/*
** mad_2_u16_x_tied1:
**	mov	(z[0-9]+\.h), #2
**	mad	z0\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_u16_x_tied1, svuint16_t,
		z0 = svmad_n_u16_x (p0, z0, z1, 2),
		z0 = svmad_x (p0, z0, z1, 2))

/*
** mad_2_u16_x_tied2:
**	mov	(z[0-9]+\.h), #2
**	mad	z1\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mad_2_u16_x_tied2, svuint16_t,
		z1 = svmad_n_u16_x (p0, z0, z1, 2),
		z1 = svmad_x (p0, z0, z1, 2))

/*
** mad_2_u16_x_untied:
**	mov	z0\.h, #2
**	mla	z0\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mad_2_u16_x_untied, svuint16_t,
		z0 = svmad_n_u16_x (p0, z1, z2, 2),
		z0 = svmad_x (p0, z1, z2, 2))

