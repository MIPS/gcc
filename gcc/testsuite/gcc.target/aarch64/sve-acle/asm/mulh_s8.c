/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_s8_m_tied1:
**	smulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_m_tied1, svint8_t,
		z0 = svmulh_s8_m (p0, z0, z1),
		z0 = svmulh_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mulh_s8_m_tied2, svint8_t,
		z1 = svmulh_s8_m (p0, z0, z1),
		z1 = svmulh_m (p0, z0, z1))

/*
** mulh_s8_m_untied:
**	movprfx	z0, z1
**	smulh	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_m_untied, svint8_t,
		z0 = svmulh_s8_m (p0, z1, z2),
		z0 = svmulh_m (p0, z1, z2))

/*
** mulh_w0_s8_m_tied1:
**	mov	(z[0-9]+\.b), w0
**	smulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s8_m_tied1, svint8_t, int8_t,
		 z0 = svmulh_n_s8_m (p0, z0, x0),
		 z0 = svmulh_m (p0, z0, x0))

/*
** mulh_w0_s8_m_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0, z1
**	smulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s8_m_untied, svint8_t, int8_t,
		 z0 = svmulh_n_s8_m (p0, z1, x0),
		 z0 = svmulh_m (p0, z1, x0))

/*
** mulh_b0_s8_m_tied1:
**	mov	(z[0-9]+\.b), b0
**	smulh	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b0_s8_m_tied1, svint8_t, int8_t,
		 z1 = svmulh_n_s8_m (p0, z1, d0),
		 z1 = svmulh_m (p0, z1, d0))

/*
** mulh_b0_s8_m_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1, z2
**	smulh	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b0_s8_m_untied, svint8_t, int8_t,
		 z1 = svmulh_n_s8_m (p0, z2, d0),
		 z1 = svmulh_m (p0, z2, d0))

/*
** mulh_2_s8_m_tied1:
**	mov	(z[0-9]+\.b), #2
**	smulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s8_m_tied1, svint8_t,
		z0 = svmulh_n_s8_m (p0, z0, 2),
		z0 = svmulh_m (p0, z0, 2))

/*
** mulh_2_s8_m_untied:
**	mov	(z[0-9]+\.b), #2
**	movprfx	z0, z1
**	smulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s8_m_untied, svint8_t,
		z0 = svmulh_n_s8_m (p0, z1, 2),
		z0 = svmulh_m (p0, z1, 2))

/*
** mulh_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	smulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_z_tied1, svint8_t,
		z0 = svmulh_s8_z (p0, z0, z1),
		z0 = svmulh_z (p0, z0, z1))

/*
** mulh_s8_z_tied2:
**	movprfx	z1\.b, p0/z, z1\.b
**	smulh	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_z_tied2, svint8_t,
		z1 = svmulh_s8_z (p0, z0, z1),
		z1 = svmulh_z (p0, z0, z1))

/*
** mulh_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	smulh	z0\.b, p0/m, z0\.b, z2\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_z_untied, svint8_t,
		z0 = svmulh_s8_z (p0, z1, z2),
		z0 = svmulh_z (p0, z1, z2))

/*
** mulh_w0_s8_z_tied1:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, z0\.b
**	smulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s8_z_tied1, svint8_t, int8_t,
		 z0 = svmulh_n_s8_z (p0, z0, x0),
		 z0 = svmulh_z (p0, z0, x0))

/*
** mulh_w0_s8_z_untied:
**	mov	(z[0-9]+\.b), w0
**	movprfx	z0\.b, p0/z, \1
**	smulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s8_z_untied, svint8_t, int8_t,
		 z0 = svmulh_n_s8_z (p0, z1, x0),
		 z0 = svmulh_z (p0, z1, x0))

/*
** mulh_b0_s8_z_tied1:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, z1\.b
**	smulh	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b0_s8_z_tied1, svint8_t, int8_t,
		 z1 = svmulh_n_s8_z (p0, z1, d0),
		 z1 = svmulh_z (p0, z1, d0))

/*
** mulh_b0_s8_z_untied:
**	mov	(z[0-9]+\.b), b0
**	movprfx	z1\.b, p0/z, \1
**	smulh	z1\.b, p0/m, z1\.b, z2\.b
**	ret
*/
TEST_UNIFORM_ZS (mulh_b0_s8_z_untied, svint8_t, int8_t,
		 z1 = svmulh_n_s8_z (p0, z2, d0),
		 z1 = svmulh_z (p0, z2, d0))

/*
** mulh_s8_x_tied1:
**	smulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_x_tied1, svint8_t,
		z0 = svmulh_s8_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_s8_x_tied2:
**	smulh	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_x_tied2, svint8_t,
		z1 = svmulh_s8_x (p0, z0, z1),
		z1 = svmulh_x (p0, z0, z1))

/*
** mulh_s8_x_untied:
**	movprfx	z2, z0
**	smulh	z2\.b, p0/m, z2\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_s8_x_untied, svint8_t,
		z2 = svmulh_s8_x (p0, z0, z1),
		z2 = svmulh_x (p0, z0, z1))

/*
** mulh_w0_s8_x_tied1:
**	mov	(z[0-9]+\.b), w0
**	smulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s8_x_tied1, svint8_t, int8_t,
		 z0 = svmulh_n_s8_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_w0_s8_x_untied:
**	mov	z1\.b, w0
**	smulh	z1\.b, p0/m, z1\.b, z0\.b
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s8_x_untied, svint8_t, int8_t,
		 z1 = svmulh_n_s8_x (p0, z0, x0),
		 z1 = svmulh_x (p0, z0, x0))

/*
** mulh_b0_s8_x_tied1:
**	mov	(z[0-9]+\.b), b0
**	smulh	z1\.b, p0/m, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_b0_s8_x_tied1, svint8_t, int8_t,
		 z1 = svmulh_n_s8_x (p0, z1, d0),
		 z1 = svmulh_x (p0, z1, d0))

/*
** mulh_b0_s8_x_untied:
**	mov	z2\.b, b0
**	smulh	z2\.b, p0/m, z2\.b, z1\.b
**	ret
*/
TEST_UNIFORM_ZS (mulh_b0_s8_x_untied, svint8_t, int8_t,
		 z2 = svmulh_n_s8_x (p0, z1, d0),
		 z2 = svmulh_x (p0, z1, d0))

/*
** mulh_2_s8_x_tied1:
**	mov	(z[0-9]+\.b), #2
**	smulh	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s8_x_tied1, svint8_t,
		z0 = svmulh_n_s8_x (p0, z0, 2),
		z0 = svmulh_x (p0, z0, 2))

/*
** mulh_2_s8_x_untied:
**	mov	z0\.b, #2
**	smulh	z0\.b, p0/m, z0\.b, z1\.b
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s8_x_untied, svint8_t,
		z0 = svmulh_n_s8_x (p0, z1, 2),
		z0 = svmulh_x (p0, z1, 2))

