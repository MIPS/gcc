/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_u16_m_tied1:
**	umulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_m_tied1, svuint16_t,
		z0 = svmulh_u16_m (p0, z0, z1),
		z0 = svmulh_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mulh_u16_m_tied2, svuint16_t,
		z1 = svmulh_u16_m (p0, z0, z1),
		z1 = svmulh_m (p0, z0, z1))

/*
** mulh_u16_m_untied:
**	movprfx	z0, z1
**	umulh	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_m_untied, svuint16_t,
		z0 = svmulh_u16_m (p0, z1, z2),
		z0 = svmulh_m (p0, z1, z2))

/*
** mulh_w0_u16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	umulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u16_m_tied1, svuint16_t, uint16_t,
		 z0 = svmulh_n_u16_m (p0, z0, x0),
		 z0 = svmulh_m (p0, z0, x0))

/*
** mulh_w0_u16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	umulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u16_m_untied, svuint16_t, uint16_t,
		 z0 = svmulh_n_u16_m (p0, z1, x0),
		 z0 = svmulh_m (p0, z1, x0))

/*
** mulh_h0_u16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	umulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_u16_m_tied1, svuint16_t, uint16_t,
		 z1 = svmulh_n_u16_m (p0, z1, d0),
		 z1 = svmulh_m (p0, z1, d0))

/*
** mulh_h0_u16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z2
**	umulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_u16_m_untied, svuint16_t, uint16_t,
		 z1 = svmulh_n_u16_m (p0, z2, d0),
		 z1 = svmulh_m (p0, z2, d0))

/*
** mulh_2_u16_m_tied1:
**	mov	(z[0-9]+\.h), #2
**	umulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u16_m_tied1, svuint16_t,
		z0 = svmulh_n_u16_m (p0, z0, 2),
		z0 = svmulh_m (p0, z0, 2))

/*
** mulh_2_u16_m_untied:
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0, z1
**	umulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u16_m_untied, svuint16_t,
		z0 = svmulh_n_u16_m (p0, z1, 2),
		z0 = svmulh_m (p0, z1, 2))

/*
** mulh_u16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	umulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_z_tied1, svuint16_t,
		z0 = svmulh_u16_z (p0, z0, z1),
		z0 = svmulh_z (p0, z0, z1))

/*
** mulh_u16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	umulh	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_z_tied2, svuint16_t,
		z1 = svmulh_u16_z (p0, z0, z1),
		z1 = svmulh_z (p0, z0, z1))

/*
** mulh_u16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	umulh	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_z_untied, svuint16_t,
		z0 = svmulh_u16_z (p0, z1, z2),
		z0 = svmulh_z (p0, z1, z2))

/*
** mulh_w0_u16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	umulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u16_z_tied1, svuint16_t, uint16_t,
		 z0 = svmulh_n_u16_z (p0, z0, x0),
		 z0 = svmulh_z (p0, z0, x0))

/*
** mulh_w0_u16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, \1
**	umulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u16_z_untied, svuint16_t, uint16_t,
		 z0 = svmulh_n_u16_z (p0, z1, x0),
		 z0 = svmulh_z (p0, z1, x0))

/*
** mulh_h0_u16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	umulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_u16_z_tied1, svuint16_t, uint16_t,
		 z1 = svmulh_n_u16_z (p0, z1, d0),
		 z1 = svmulh_z (p0, z1, d0))

/*
** mulh_h0_u16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, \1
**	umulh	z1\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_u16_z_untied, svuint16_t, uint16_t,
		 z1 = svmulh_n_u16_z (p0, z2, d0),
		 z1 = svmulh_z (p0, z2, d0))

/*
** mulh_u16_x_tied1:
**	umulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_x_tied1, svuint16_t,
		z0 = svmulh_u16_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_u16_x_tied2:
**	umulh	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_x_tied2, svuint16_t,
		z1 = svmulh_u16_x (p0, z0, z1),
		z1 = svmulh_x (p0, z0, z1))

/*
** mulh_u16_x_untied:
**	movprfx	z2, z0
**	umulh	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_u16_x_untied, svuint16_t,
		z2 = svmulh_u16_x (p0, z0, z1),
		z2 = svmulh_x (p0, z0, z1))

/*
** mulh_w0_u16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	umulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u16_x_tied1, svuint16_t, uint16_t,
		 z0 = svmulh_n_u16_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_w0_u16_x_untied:
**	mov	z1\.h, w0
**	umulh	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u16_x_untied, svuint16_t, uint16_t,
		 z1 = svmulh_n_u16_x (p0, z0, x0),
		 z1 = svmulh_x (p0, z0, x0))

/*
** mulh_h0_u16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	umulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_u16_x_tied1, svuint16_t, uint16_t,
		 z1 = svmulh_n_u16_x (p0, z1, d0),
		 z1 = svmulh_x (p0, z1, d0))

/*
** mulh_h0_u16_x_untied:
**	mov	z2\.h, h0
**	umulh	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_u16_x_untied, svuint16_t, uint16_t,
		 z2 = svmulh_n_u16_x (p0, z1, d0),
		 z2 = svmulh_x (p0, z1, d0))

/*
** mulh_2_u16_x_tied1:
**	mov	(z[0-9]+\.h), #2
**	umulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u16_x_tied1, svuint16_t,
		z0 = svmulh_n_u16_x (p0, z0, 2),
		z0 = svmulh_x (p0, z0, 2))

/*
** mulh_2_u16_x_untied:
**	mov	z0\.h, #2
**	umulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u16_x_untied, svuint16_t,
		z0 = svmulh_n_u16_x (p0, z1, 2),
		z0 = svmulh_x (p0, z1, 2))

