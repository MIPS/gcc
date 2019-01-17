/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_s16_m_tied1:
**	smulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_m_tied1, svint16_t,
		z0 = svmulh_s16_m (p0, z0, z1),
		z0 = svmulh_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mulh_s16_m_tied2, svint16_t,
		z1 = svmulh_s16_m (p0, z0, z1),
		z1 = svmulh_m (p0, z0, z1))

/*
** mulh_s16_m_untied:
**	movprfx	z0, z1
**	smulh	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_m_untied, svint16_t,
		z0 = svmulh_s16_m (p0, z1, z2),
		z0 = svmulh_m (p0, z1, z2))

/*
** mulh_w0_s16_m_tied1:
**	mov	(z[0-9]+\.h), w0
**	smulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s16_m_tied1, svint16_t, int16_t,
		 z0 = svmulh_n_s16_m (p0, z0, x0),
		 z0 = svmulh_m (p0, z0, x0))

/*
** mulh_w0_s16_m_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	smulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s16_m_untied, svint16_t, int16_t,
		 z0 = svmulh_n_s16_m (p0, z1, x0),
		 z0 = svmulh_m (p0, z1, x0))

/*
** mulh_h0_s16_m_tied1:
**	mov	(z[0-9]+\.h), h0
**	smulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_s16_m_tied1, svint16_t, int16_t,
		 z1 = svmulh_n_s16_m (p0, z1, d0),
		 z1 = svmulh_m (p0, z1, d0))

/*
** mulh_h0_s16_m_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1, z2
**	smulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_s16_m_untied, svint16_t, int16_t,
		 z1 = svmulh_n_s16_m (p0, z2, d0),
		 z1 = svmulh_m (p0, z2, d0))

/*
** mulh_2_s16_m_tied1:
**	mov	(z[0-9]+\.h), #2
**	smulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s16_m_tied1, svint16_t,
		z0 = svmulh_n_s16_m (p0, z0, 2),
		z0 = svmulh_m (p0, z0, 2))

/*
** mulh_2_s16_m_untied:
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0, z1
**	smulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s16_m_untied, svint16_t,
		z0 = svmulh_n_s16_m (p0, z1, 2),
		z0 = svmulh_m (p0, z1, 2))

/*
** mulh_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	smulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_z_tied1, svint16_t,
		z0 = svmulh_s16_z (p0, z0, z1),
		z0 = svmulh_z (p0, z0, z1))

/*
** mulh_s16_z_tied2:
**	movprfx	z1\.h, p0/z, z1\.h
**	smulh	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_z_tied2, svint16_t,
		z1 = svmulh_s16_z (p0, z0, z1),
		z1 = svmulh_z (p0, z0, z1))

/*
** mulh_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	smulh	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_z_untied, svint16_t,
		z0 = svmulh_s16_z (p0, z1, z2),
		z0 = svmulh_z (p0, z1, z2))

/*
** mulh_w0_s16_z_tied1:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, z0\.h
**	smulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s16_z_tied1, svint16_t, int16_t,
		 z0 = svmulh_n_s16_z (p0, z0, x0),
		 z0 = svmulh_z (p0, z0, x0))

/*
** mulh_w0_s16_z_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0\.h, p0/z, \1
**	smulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s16_z_untied, svint16_t, int16_t,
		 z0 = svmulh_n_s16_z (p0, z1, x0),
		 z0 = svmulh_z (p0, z1, x0))

/*
** mulh_h0_s16_z_tied1:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, z1\.h
**	smulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_s16_z_tied1, svint16_t, int16_t,
		 z1 = svmulh_n_s16_z (p0, z1, d0),
		 z1 = svmulh_z (p0, z1, d0))

/*
** mulh_h0_s16_z_untied:
**	mov	(z[0-9]+\.h), h0
**	movprfx	z1\.h, p0/z, \1
**	smulh	z1\.h, p0/m, z1\.h, z2\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_s16_z_untied, svint16_t, int16_t,
		 z1 = svmulh_n_s16_z (p0, z2, d0),
		 z1 = svmulh_z (p0, z2, d0))

/*
** mulh_s16_x_tied1:
**	smulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_x_tied1, svint16_t,
		z0 = svmulh_s16_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_s16_x_tied2:
**	smulh	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_x_tied2, svint16_t,
		z1 = svmulh_s16_x (p0, z0, z1),
		z1 = svmulh_x (p0, z0, z1))

/*
** mulh_s16_x_untied:
**	movprfx	z2, z0
**	smulh	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_s16_x_untied, svint16_t,
		z2 = svmulh_s16_x (p0, z0, z1),
		z2 = svmulh_x (p0, z0, z1))

/*
** mulh_w0_s16_x_tied1:
**	mov	(z[0-9]+\.h), w0
**	smulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s16_x_tied1, svint16_t, int16_t,
		 z0 = svmulh_n_s16_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_w0_s16_x_untied:
**	mov	z1\.h, w0
**	smulh	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s16_x_untied, svint16_t, int16_t,
		 z1 = svmulh_n_s16_x (p0, z0, x0),
		 z1 = svmulh_x (p0, z0, x0))

/*
** mulh_h0_s16_x_tied1:
**	mov	(z[0-9]+\.h), h0
**	smulh	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_s16_x_tied1, svint16_t, int16_t,
		 z1 = svmulh_n_s16_x (p0, z1, d0),
		 z1 = svmulh_x (p0, z1, d0))

/*
** mulh_h0_s16_x_untied:
**	mov	z2\.h, h0
**	smulh	z2\.h, p0/m, z2\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (mulh_h0_s16_x_untied, svint16_t, int16_t,
		 z2 = svmulh_n_s16_x (p0, z1, d0),
		 z2 = svmulh_x (p0, z1, d0))

/*
** mulh_2_s16_x_tied1:
**	mov	(z[0-9]+\.h), #2
**	smulh	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s16_x_tied1, svint16_t,
		z0 = svmulh_n_s16_x (p0, z0, 2),
		z0 = svmulh_x (p0, z0, 2))

/*
** mulh_2_s16_x_untied:
**	mov	z0\.h, #2
**	smulh	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s16_x_untied, svint16_t,
		z0 = svmulh_n_s16_x (p0, z1, 2),
		z0 = svmulh_x (p0, z1, 2))

