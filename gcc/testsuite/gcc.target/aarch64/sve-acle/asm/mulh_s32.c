/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** mulh_s32_m_tied1:
**	smulh	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_m_tied1, svint32_t,
		z0 = svmulh_s32_m (p0, z0, z1),
		z0 = svmulh_m (p0, z0, z1))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (mulh_s32_m_tied2, svint32_t,
		z1 = svmulh_s32_m (p0, z0, z1),
		z1 = svmulh_m (p0, z0, z1))

/*
** mulh_s32_m_untied:
**	movprfx	z0, z1
**	smulh	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_m_untied, svint32_t,
		z0 = svmulh_s32_m (p0, z1, z2),
		z0 = svmulh_m (p0, z1, z2))

/*
** mulh_w0_s32_m_tied1:
**	mov	(z[0-9]+\.s), w0
**	smulh	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s32_m_tied1, svint32_t, int32_t,
		 z0 = svmulh_n_s32_m (p0, z0, x0),
		 z0 = svmulh_m (p0, z0, x0))

/*
** mulh_w0_s32_m_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	smulh	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s32_m_untied, svint32_t, int32_t,
		 z0 = svmulh_n_s32_m (p0, z1, x0),
		 z0 = svmulh_m (p0, z1, x0))

/*
** mulh_s0_s32_m_tied1:
**	mov	(z[0-9]+\.s), s0
**	smulh	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_s0_s32_m_tied1, svint32_t, int32_t,
		 z1 = svmulh_n_s32_m (p0, z1, d0),
		 z1 = svmulh_m (p0, z1, d0))

/*
** mulh_s0_s32_m_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	smulh	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_s0_s32_m_untied, svint32_t, int32_t,
		 z1 = svmulh_n_s32_m (p0, z2, d0),
		 z1 = svmulh_m (p0, z2, d0))

/*
** mulh_2_s32_m_tied1:
**	mov	(z[0-9]+\.s), #2
**	smulh	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s32_m_tied1, svint32_t,
		z0 = svmulh_n_s32_m (p0, z0, 2),
		z0 = svmulh_m (p0, z0, 2))

/*
** mulh_2_s32_m_untied:
**	mov	(z[0-9]+\.s), #2
**	movprfx	z0, z1
**	smulh	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s32_m_untied, svint32_t,
		z0 = svmulh_n_s32_m (p0, z1, 2),
		z0 = svmulh_m (p0, z1, 2))

/*
** mulh_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	smulh	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_z_tied1, svint32_t,
		z0 = svmulh_s32_z (p0, z0, z1),
		z0 = svmulh_z (p0, z0, z1))

/*
** mulh_s32_z_tied2:
**	movprfx	z1\.s, p0/z, z1\.s
**	smulh	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_z_tied2, svint32_t,
		z1 = svmulh_s32_z (p0, z0, z1),
		z1 = svmulh_z (p0, z0, z1))

/*
** mulh_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	smulh	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_z_untied, svint32_t,
		z0 = svmulh_s32_z (p0, z1, z2),
		z0 = svmulh_z (p0, z1, z2))

/*
** mulh_w0_s32_z_tied1:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	smulh	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s32_z_tied1, svint32_t, int32_t,
		 z0 = svmulh_n_s32_z (p0, z0, x0),
		 z0 = svmulh_z (p0, z0, x0))

/*
** mulh_w0_s32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, \1
**	smulh	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s32_z_untied, svint32_t, int32_t,
		 z0 = svmulh_n_s32_z (p0, z1, x0),
		 z0 = svmulh_z (p0, z1, x0))

/*
** mulh_s0_s32_z_tied1:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	smulh	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_s0_s32_z_tied1, svint32_t, int32_t,
		 z1 = svmulh_n_s32_z (p0, z1, d0),
		 z1 = svmulh_z (p0, z1, d0))

/*
** mulh_s0_s32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, \1
**	smulh	z1\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (mulh_s0_s32_z_untied, svint32_t, int32_t,
		 z1 = svmulh_n_s32_z (p0, z2, d0),
		 z1 = svmulh_z (p0, z2, d0))

/*
** mulh_s32_x_tied1:
**	smulh	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_x_tied1, svint32_t,
		z0 = svmulh_s32_x (p0, z0, z1),
		z0 = svmulh_x (p0, z0, z1))

/*
** mulh_s32_x_tied2:
**	smulh	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_x_tied2, svint32_t,
		z1 = svmulh_s32_x (p0, z0, z1),
		z1 = svmulh_x (p0, z0, z1))

/*
** mulh_s32_x_untied:
**	movprfx	z2, z0
**	smulh	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_s32_x_untied, svint32_t,
		z2 = svmulh_s32_x (p0, z0, z1),
		z2 = svmulh_x (p0, z0, z1))

/*
** mulh_w0_s32_x_tied1:
**	mov	(z[0-9]+\.s), w0
**	smulh	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s32_x_tied1, svint32_t, int32_t,
		 z0 = svmulh_n_s32_x (p0, z0, x0),
		 z0 = svmulh_x (p0, z0, x0))

/*
** mulh_w0_s32_x_untied:
**	mov	z1\.s, w0
**	smulh	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (mulh_w0_s32_x_untied, svint32_t, int32_t,
		 z1 = svmulh_n_s32_x (p0, z0, x0),
		 z1 = svmulh_x (p0, z0, x0))

/*
** mulh_s0_s32_x_tied1:
**	mov	(z[0-9]+\.s), s0
**	smulh	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (mulh_s0_s32_x_tied1, svint32_t, int32_t,
		 z1 = svmulh_n_s32_x (p0, z1, d0),
		 z1 = svmulh_x (p0, z1, d0))

/*
** mulh_s0_s32_x_untied:
**	mov	z2\.s, s0
**	smulh	z2\.s, p0/m, z2\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (mulh_s0_s32_x_untied, svint32_t, int32_t,
		 z2 = svmulh_n_s32_x (p0, z1, d0),
		 z2 = svmulh_x (p0, z1, d0))

/*
** mulh_2_s32_x_tied1:
**	mov	(z[0-9]+\.s), #2
**	smulh	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s32_x_tied1, svint32_t,
		z0 = svmulh_n_s32_x (p0, z0, 2),
		z0 = svmulh_x (p0, z0, 2))

/*
** mulh_2_s32_x_untied:
**	mov	z0\.s, #2
**	smulh	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (mulh_2_s32_x_untied, svint32_t,
		z0 = svmulh_n_s32_x (p0, z1, 2),
		z0 = svmulh_x (p0, z1, 2))

