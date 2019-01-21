/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_u8_m_tied1:
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_m_tied1, svuint8_t,
		z0 = svmulh_u8_m (p0, z0, z1),
		z0 = svmulh_m (p0, z0, z1))

/*
** mulh_u8_m_tied2:
**	mov	(z[0-9]+)\.d, z0\.d
**	movprfx	z0, z1
**	umulh	z0\.b, p0/m, z0\.b, \1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_m_tied2, svuint8_t,
		z0 = svmulh_u8_m (p0, z1, z0),
		z0 = svmulh_m (p0, z1, z0))

/*
** mulh_u8_m_untied:
**	movprfx	z0, z1
**	umulh	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_m_untied, svuint8_t,
		z0 = svmulh_u8_m (p0, z1, z2),
		z0 = svmulh_m (p0, z1, z2))

/*
** mulh_w0_u8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u8_m_tied1, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_m (p0, z0, x0),
		 z0 = svmulh_m (p0, z0, x0))

/*
** mulh_w0_u8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u8_m_untied, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_m (p0, z1, x0),
		 z0 = svmulh_m (p0, z1, x0))

/*
** mulh_b4_u8_m_tied1:
**	mov	(z[0-9]+\.b), b4
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b4_u8_m_tied1, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_m (p0, z0, d4),
		 z0 = svmulh_m (p0, z0, d4))

/*
** mulh_b4_u8_m_untied:
**	mov	(z[0-9]+\.b), b4
**	movprfx	z0, z1
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b4_u8_m_untied, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_m (p0, z1, d4),
		 z0 = svmulh_m (p0, z1, d4))

/*
** mulh_2_u8_m_tied1:
**	mov	(z[0-9]+\.b), #2
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u8_m_tied1, svuint8_t,
		z0 = svmulh_n_u8_m (p0, z0, 2),
		z0 = svmulh_m (p0, z0, 2))

/*
** mulh_2_u8_m_untied:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0, z1
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u8_m_untied, svuint8_t,
		z0 = svmulh_n_u8_m (p0, z1, 2),
		z0 = svmulh_m (p0, z1, 2))

/*
** mulh_u8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_z_tied1, svuint8_t,
		z0 = svmulh_u8_z (p0, z0, z1),
		z0 = svmulh_z (p0, z0, z1))

/*
** mulh_u8_z_tied2:
**	movprfx	z0\.b, p0/z, z0\.b
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_z_tied2, svuint8_t,
		z0 = svmulh_u8_z (p0, z1, z0),
		z0 = svmulh_z (p0, z1, z0))

/*
** mulh_u8_z_untied:
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	umulh	z0\.b, p0/m, z0\.b, z2\.b
** |
**	movprfx	z0\.b, p0/z, z2\.b
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_z_untied, svuint8_t,
		z0 = svmulh_u8_z (p0, z1, z2),
		z0 = svmulh_z (p0, z1, z2))

/*
** mulh_w0_u8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u8_z_tied1, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_z (p0, z0, x0),
		 z0 = svmulh_z (p0, z0, x0))

/*
** mulh_w0_u8_z_untied:
**	mov	(z[0-9]+\.b), w0
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	umulh	z0\.b, p0/m, z0\.b, \1
** |
**	movprfx	z0\.b, p0/z, \1
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u8_z_untied, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_z (p0, z1, x0),
		 z0 = svmulh_z (p0, z1, x0))

/*
** mulh_b4_u8_z_tied1:
**	mov	(z[0-9]+\.b), b4
**	movprfx	z0\.b, p0/z, z0\.b
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b4_u8_z_tied1, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_z (p0, z0, d4),
		 z0 = svmulh_z (p0, z0, d4))

/*
** mulh_b4_u8_z_untied:
**	mov	(z[0-9]+\.b), b4
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	umulh	z0\.b, p0/m, z0\.b, \1
** |
**	movprfx	z0\.b, p0/z, \1
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_ZS (mulh_b4_u8_z_untied, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_z (p0, z1, d4),
		 z0 = svmulh_z (p0, z1, d4))

/*
** mulh_2_u8_z_tied1:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0\.b, p0/z, z0\.b
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u8_z_tied1, svuint8_t,
		z0 = svmulh_n_u8_z (p0, z0, 2),
		z0 = svmulh_z (p0, z0, 2))

/*
** mulh_2_u8_z_untied:
**	mov	(z[0-9]+\.b), #2
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	umulh	z0\.b, p0/m, z0\.b, \1
** |
**	movprfx	z0\.b, p0/z, \1
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u8_z_untied, svuint8_t,
		z0 = svmulh_n_u8_z (p0, z1, 2),
		z0 = svmulh_z (p0, z1, 2))

/*
** mulh_u8_x_tied1:
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_x_tied1, svuint8_t,
		z0 = svmulh_u8_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_u8_x_tied2:
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_x_tied2, svuint8_t,
		z0 = svmulh_u8_x (p0, z1, z0),
		z0 = svmulh_x (p0, z1, z0))

/*
** mulh_u8_x_untied:
** (
**	movprfx	z0, z1
**	umulh	z0\.b, p0/m, z0\.b, z2\.b
** |
**	movprfx	z0, z2
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_UNIFORM_Z (mulh_u8_x_untied, svuint8_t,
		z0 = svmulh_u8_x (p0, z1, z2),
		z0 = svmulh_x (p0, z1, z2))

/*
** mulh_w0_u8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u8_x_tied1, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_w0_u8_x_untied:
**	mov	z0\.b, w0
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_u8_x_untied, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_x (p0, z1, x0),
		 z0 = svmulh_x (p0, z1, x0))

/*
** mulh_b4_u8_x_tied1:
**	mov	(z[0-9]+\.b), b4
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b4_u8_x_tied1, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_x (p0, z0, d4),
		 z0 = svmulh_x (p0, z0, d4))

/*
** mulh_b4_u8_x_untied:
**	mov	z0\.b, b4
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (mulh_b4_u8_x_untied, svuint8_t, uint8_t,
		 z0 = svmulh_n_u8_x (p0, z1, d4),
		 z0 = svmulh_x (p0, z1, d4))

/*
** mulh_2_u8_x_tied1:
**	mov	(z[0-9]+\.b), #2
**	umulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u8_x_tied1, svuint8_t,
		z0 = svmulh_n_u8_x (p0, z0, 2),
		z0 = svmulh_x (p0, z0, 2))

/*
** mulh_2_u8_x_untied:
**	mov	z0\.b, #2
**	umulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_2_u8_x_untied, svuint8_t,
		z0 = svmulh_n_u8_x (p0, z1, 2),
		z0 = svmulh_x (p0, z1, 2))
