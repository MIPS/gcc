/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_s8_m_tied1:
**	lsl	z0\.b, p0/m, z0\.b, z16\.b
**	ret
*/
TEST_DUAL_Z (lsl_s8_m_tied1, svint8_t, svuint8_t,
	     z0 = svlsl_s8_m (p0, z0, z16),
	     z0 = svlsl_m (p0, z0, z16))

/*
** lsl_s8_m_tied2:
**	mov	(z[0-9]+)\.d, z16\.d
**	movprfx	z16, z0
**	lsl	z16\.b, p0/m, z16\.b, \1\.b
**	ret
*/
TEST_DUAL_Z (lsl_s8_m_tied2, svint8_t, svuint8_t,
	     z16_res = svlsl_s8_m (p0, z0, z16),
	     z16_res = svlsl_m (p0, z0, z16))

/*
** lsl_s8_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, z16\.b
**	ret
*/
TEST_DUAL_Z (lsl_s8_m_untied, svint8_t, svuint8_t,
	     z0 = svlsl_s8_m (p0, z1, z16),
	     z0 = svlsl_m (p0, z1, z16))

/*
** lsl_x0_s8_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s8_m_tied1, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_m (p0, z0, x0),
		 z0 = svlsl_m (p0, z0, x0))

/*
** lsl_x0_s8_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s8_m_untied, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_m (p0, z1, x0),
		 z0 = svlsl_m (p0, z1, x0))

/*
** lsl_d4_s8_m_tied1:
**	mov	(z[0-9]+\.d), d4
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s8_m_tied1, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_m (p0, z0, d4),
		 z0 = svlsl_m (p0, z0, d4))

/*
** lsl_d4_s8_m_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s8_m_untied, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_m (p0, z1, d4),
		 z0 = svlsl_m (p0, z1, d4))

/*
** lsl_1_s8_m_tied1:
**	lsl	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s8_m_tied1, svint8_t,
		z0 = svlsl_n_s8_m (p0, z0, 1),
		z0 = svlsl_m (p0, z0, 1))

/*
** lsl_1_s8_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s8_m_untied, svint8_t,
		z0 = svlsl_n_s8_m (p0, z1, 1),
		z0 = svlsl_m (p0, z1, 1))

/*
** lsl_7_s8_m_tied1:
**	lsl	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (lsl_7_s8_m_tied1, svint8_t,
		z0 = svlsl_n_s8_m (p0, z0, 7),
		z0 = svlsl_m (p0, z0, 7))

/*
** lsl_7_s8_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (lsl_7_s8_m_untied, svint8_t,
		z0 = svlsl_n_s8_m (p0, z1, 7),
		z0 = svlsl_m (p0, z1, 7))

/*
** lsl_8_s8_m_tied1:
**	mov	(z[0-9]+\.d), #8
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_8_s8_m_tied1, svint8_t,
		z0 = svlsl_n_s8_m (p0, z0, 8),
		z0 = svlsl_m (p0, z0, 8))

/*
** lsl_8_s8_m_untied:
**	mov	(z[0-9]+\.d), #8
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_8_s8_m_untied, svint8_t,
		z0 = svlsl_n_s8_m (p0, z1, 8),
		z0 = svlsl_m (p0, z1, 8))

/*
** lsl_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, z16\.b
**	ret
*/
TEST_DUAL_Z (lsl_s8_z_tied1, svint8_t, svuint8_t,
	     z0 = svlsl_s8_z (p0, z0, z16),
	     z0 = svlsl_z (p0, z0, z16))

/*
** lsl_s8_z_tied2:
**	movprfx	z16\.b, p0/z, z16\.b
**	lslr	z16\.b, p0/m, z16\.b, z0\.b
**	ret
*/
TEST_DUAL_Z (lsl_s8_z_tied2, svint8_t, svuint8_t,
	     z16_res = svlsl_s8_z (p0, z0, z16),
	     z16_res = svlsl_z (p0, z0, z16))

/*
** lsl_s8_z_untied:
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, z16\.b
** |
**	movprfx	z0\.b, p0/z, z16\.b
**	lslr	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_DUAL_Z (lsl_s8_z_untied, svint8_t, svuint8_t,
	     z0 = svlsl_s8_z (p0, z1, z16),
	     z0 = svlsl_z (p0, z1, z16))

/*
** lsl_x0_s8_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s8_z_tied1, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_z (p0, z0, x0),
		 z0 = svlsl_z (p0, z0, x0))

/*
** lsl_x0_s8_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s8_z_untied, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_z (p0, z1, x0),
		 z0 = svlsl_z (p0, z1, x0))

/*
** lsl_d4_s8_z_tied1:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s8_z_tied1, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_z (p0, z0, d4),
		 z0 = svlsl_z (p0, z0, d4))

/*
** lsl_d4_s8_z_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s8_z_untied, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_z (p0, z1, d4),
		 z0 = svlsl_z (p0, z1, d4))

/*
** lsl_1_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s8_z_tied1, svint8_t,
		z0 = svlsl_n_s8_z (p0, z0, 1),
		z0 = svlsl_z (p0, z0, 1))

/*
** lsl_1_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s8_z_untied, svint8_t,
		z0 = svlsl_n_s8_z (p0, z1, 1),
		z0 = svlsl_z (p0, z1, 1))

/*
** lsl_7_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (lsl_7_s8_z_tied1, svint8_t,
		z0 = svlsl_n_s8_z (p0, z0, 7),
		z0 = svlsl_z (p0, z0, 7))

/*
** lsl_7_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (lsl_7_s8_z_untied, svint8_t,
		z0 = svlsl_n_s8_z (p0, z1, 7),
		z0 = svlsl_z (p0, z1, 7))

/*
** lsl_8_s8_z_tied1:
**	mov	(z[0-9]+\.d), #8
**	movprfx	z0\.b, p0/z, z0\.b
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_8_s8_z_tied1, svint8_t,
		z0 = svlsl_n_s8_z (p0, z0, 8),
		z0 = svlsl_z (p0, z0, 8))

/*
** lsl_8_s8_z_untied:
**	mov	(z[0-9]+\.d), #8
**	movprfx	z0\.b, p0/z, z1\.b
**	lsl	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_8_s8_z_untied, svint8_t,
		z0 = svlsl_n_s8_z (p0, z1, 8),
		z0 = svlsl_z (p0, z1, 8))

/*
** lsl_s8_x_tied1:
**	lsl	z0\.b, p0/m, z0\.b, z16\.b
**	ret
*/
TEST_DUAL_Z (lsl_s8_x_tied1, svint8_t, svuint8_t,
	     z0 = svlsl_s8_x (p0, z0, z16),
	     z0 = svlsl_x (p0, z0, z16))

/*
** lsl_s8_x_tied2:
**	lslr	z16\.b, p0/m, z16\.b, z0\.b
**	ret
*/
TEST_DUAL_Z (lsl_s8_x_tied2, svint8_t, svuint8_t,
	     z16_res = svlsl_s8_x (p0, z0, z16),
	     z16_res = svlsl_x (p0, z0, z16))

/*
** lsl_s8_x_untied:
** (
**	movprfx	z0, z1
**	lsl	z0\.b, p0/m, z0\.b, z16\.b
** |
**	movprfx	z0, z16
**	lslr	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_DUAL_Z (lsl_s8_x_untied, svint8_t, svuint8_t,
	     z0 = svlsl_s8_x (p0, z1, z16),
	     z0 = svlsl_x (p0, z1, z16))

/*
** lsl_x0_s8_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s8_x_tied1, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_x (p0, z0, x0),
		 z0 = svlsl_x (p0, z0, x0))

/*
** lsl_x0_s8_x_untied:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s8_x_untied, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_x (p0, z1, x0),
		 z0 = svlsl_x (p0, z1, x0))

/*
** lsl_d4_s8_x_tied1:
**	mov	(z[0-9]+\.d), d4
**	lsl	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s8_x_tied1, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_x (p0, z0, d4),
		 z0 = svlsl_x (p0, z0, d4))

/*
** lsl_d4_s8_x_untied:
**	mov	(z[0-9]+\.d), d4
**	lsl	z0\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s8_x_untied, svint8_t, uint64_t,
		 z0 = svlsl_n_s8_x (p0, z1, d4),
		 z0 = svlsl_x (p0, z1, d4))

/*
** lsl_1_s8_x_tied1:
**	lsl	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s8_x_tied1, svint8_t,
		z0 = svlsl_n_s8_x (p0, z0, 1),
		z0 = svlsl_x (p0, z0, 1))

/*
** lsl_1_s8_x_untied:
**	lsl	z0\.b, z1\.b, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s8_x_untied, svint8_t,
		z0 = svlsl_n_s8_x (p0, z1, 1),
		z0 = svlsl_x (p0, z1, 1))

/*
** lsl_7_s8_x_tied1:
**	lsl	z0\.b, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (lsl_7_s8_x_tied1, svint8_t,
		z0 = svlsl_n_s8_x (p0, z0, 7),
		z0 = svlsl_x (p0, z0, 7))

/*
** lsl_7_s8_x_untied:
**	lsl	z0\.b, z1\.b, #7
**	ret
*/
TEST_UNIFORM_Z (lsl_7_s8_x_untied, svint8_t,
		z0 = svlsl_n_s8_x (p0, z1, 7),
		z0 = svlsl_x (p0, z1, 7))

/*
** lsl_8_s8_x_tied1:
**	mov	(z[0-9]+\.d), #8
**	lsl	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_8_s8_x_tied1, svint8_t,
		z0 = svlsl_n_s8_x (p0, z0, 8),
		z0 = svlsl_x (p0, z0, 8))

/*
** lsl_8_s8_x_untied:
**	mov	(z[0-9]+\.d), #8
**	lsl	z0\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_8_s8_x_untied, svint8_t,
		z0 = svlsl_n_s8_x (p0, z1, 8),
		z0 = svlsl_x (p0, z1, 8))
