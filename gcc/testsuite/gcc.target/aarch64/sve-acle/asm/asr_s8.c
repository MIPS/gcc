/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** asr_s8_m_tied1:
**	asr	z0\.b, p0/m, z0\.b, z6\.b
**	ret
*/
TEST_DUAL_Z (asr_s8_m_tied1, svint8_t, svuint8_t,
	     z0 = svasr_s8_m (p0, z0, z6),
	     z0 = svasr_m (p0, z0, z6))

/*
** asr_s8_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	asr	z6\.b, p0/m, z6\.b, \1\.b
**	ret
*/
TEST_DUAL_Z (asr_s8_m_tied2, svint8_t, svuint8_t,
	     z6_res = svasr_s8_m (p0, z0, z6),
	     z6_res = svasr_m (p0, z0, z6))

/*
** asr_s8_m_untied:
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, z6\.b
**	ret
*/
TEST_DUAL_Z (asr_s8_m_untied, svint8_t, svuint8_t,
	     z0 = svasr_s8_m (p0, z1, z6),
	     z0 = svasr_m (p0, z1, z6))

/*
** asr_x0_s8_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s8_m_tied1, svint8_t, uint64_t,
		 z0 = svasr_n_s8_m (p0, z0, x0),
		 z0 = svasr_m (p0, z0, x0))

/*
** asr_x0_s8_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s8_m_untied, svint8_t, uint64_t,
		 z0 = svasr_n_s8_m (p0, z1, x0),
		 z0 = svasr_m (p0, z1, x0))

/*
** asr_d4_s8_m_tied1:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s8_m_tied1, svint8_t, uint64_t,
		 z0 = svasr_n_s8_m (p0, z0, d4),
		 z0 = svasr_m (p0, z0, d4))

/*
** asr_d4_s8_m_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s8_m_untied, svint8_t, uint64_t,
		 z0 = svasr_n_s8_m (p0, z1, d4),
		 z0 = svasr_m (p0, z1, d4))

/*
** asr_1_s8_m_tied1:
**	asr	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s8_m_tied1, svint8_t,
		z0 = svasr_n_s8_m (p0, z0, 1),
		z0 = svasr_m (p0, z0, 1))

/*
** asr_1_s8_m_untied:
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s8_m_untied, svint8_t,
		z0 = svasr_n_s8_m (p0, z1, 1),
		z0 = svasr_m (p0, z1, 1))

/*
** asr_7_s8_m_tied1:
**	asr	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (asr_7_s8_m_tied1, svint8_t,
		z0 = svasr_n_s8_m (p0, z0, 7),
		z0 = svasr_m (p0, z0, 7))

/*
** asr_7_s8_m_untied:
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (asr_7_s8_m_untied, svint8_t,
		z0 = svasr_n_s8_m (p0, z1, 7),
		z0 = svasr_m (p0, z1, 7))

/*
** asr_8_s8_m_tied1:
**	asr	z0\.b, p0/m, z0\.b, #8
**	ret
*/
TEST_UNIFORM_Z (asr_8_s8_m_tied1, svint8_t,
		z0 = svasr_n_s8_m (p0, z0, 8),
		z0 = svasr_m (p0, z0, 8))

/*
** asr_8_s8_m_untied:
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, #8
**	ret
*/
TEST_UNIFORM_Z (asr_8_s8_m_untied, svint8_t,
		z0 = svasr_n_s8_m (p0, z1, 8),
		z0 = svasr_m (p0, z1, 8))

/*
** asr_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	asr	z0\.b, p0/m, z0\.b, z6\.b
**	ret
*/
TEST_DUAL_Z (asr_s8_z_tied1, svint8_t, svuint8_t,
	     z0 = svasr_s8_z (p0, z0, z6),
	     z0 = svasr_z (p0, z0, z6))

/*
** asr_s8_z_tied2:
**	movprfx	z6\.b, p0/z, z6\.b
**	asrr	z6\.b, p0/m, z6\.b, z0\.b
**	ret
*/
TEST_DUAL_Z (asr_s8_z_tied2, svint8_t, svuint8_t,
	     z6_res = svasr_s8_z (p0, z0, z6),
	     z6_res = svasr_z (p0, z0, z6))

/*
** asr_s8_z_untied:
** (
**	movprfx	z0\.b, p0/z, z1\.b
**	asr	z0\.b, p0/m, z0\.b, z6\.b
** |
**	movprfx	z0\.b, p0/z, z6\.b
**	asrr	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_DUAL_Z (asr_s8_z_untied, svint8_t, svuint8_t,
	     z0 = svasr_s8_z (p0, z1, z6),
	     z0 = svasr_z (p0, z1, z6))

/*
** asr_x0_s8_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.b, p0/z, z0\.b
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s8_z_tied1, svint8_t, uint64_t,
		 z0 = svasr_n_s8_z (p0, z0, x0),
		 z0 = svasr_z (p0, z0, x0))

/*
** asr_x0_s8_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.b, p0/z, z1\.b
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s8_z_untied, svint8_t, uint64_t,
		 z0 = svasr_n_s8_z (p0, z1, x0),
		 z0 = svasr_z (p0, z1, x0))

/*
** asr_d4_s8_z_tied1:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.b, p0/z, z0\.b
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s8_z_tied1, svint8_t, uint64_t,
		 z0 = svasr_n_s8_z (p0, z0, d4),
		 z0 = svasr_z (p0, z0, d4))

/*
** asr_d4_s8_z_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.b, p0/z, z1\.b
**	asr	z0\.b, p0/m, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s8_z_untied, svint8_t, uint64_t,
		 z0 = svasr_n_s8_z (p0, z1, d4),
		 z0 = svasr_z (p0, z1, d4))

/*
** asr_1_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	asr	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s8_z_tied1, svint8_t,
		z0 = svasr_n_s8_z (p0, z0, 1),
		z0 = svasr_z (p0, z0, 1))

/*
** asr_1_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	asr	z0\.b, p0/m, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s8_z_untied, svint8_t,
		z0 = svasr_n_s8_z (p0, z1, 1),
		z0 = svasr_z (p0, z1, 1))

/*
** asr_7_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	asr	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (asr_7_s8_z_tied1, svint8_t,
		z0 = svasr_n_s8_z (p0, z0, 7),
		z0 = svasr_z (p0, z0, 7))

/*
** asr_7_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	asr	z0\.b, p0/m, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (asr_7_s8_z_untied, svint8_t,
		z0 = svasr_n_s8_z (p0, z1, 7),
		z0 = svasr_z (p0, z1, 7))

/*
** asr_8_s8_z_tied1:
**	movprfx	z0\.b, p0/z, z0\.b
**	asr	z0\.b, p0/m, z0\.b, #8
**	ret
*/
TEST_UNIFORM_Z (asr_8_s8_z_tied1, svint8_t,
		z0 = svasr_n_s8_z (p0, z0, 8),
		z0 = svasr_z (p0, z0, 8))

/*
** asr_8_s8_z_untied:
**	movprfx	z0\.b, p0/z, z1\.b
**	asr	z0\.b, p0/m, z0\.b, #8
**	ret
*/
TEST_UNIFORM_Z (asr_8_s8_z_untied, svint8_t,
		z0 = svasr_n_s8_z (p0, z1, 8),
		z0 = svasr_z (p0, z1, 8))

/*
** asr_s8_x_tied1:
**	asr	z0\.b, p0/m, z0\.b, z6\.b
**	ret
*/
TEST_DUAL_Z (asr_s8_x_tied1, svint8_t, svuint8_t,
	     z0 = svasr_s8_x (p0, z0, z6),
	     z0 = svasr_x (p0, z0, z6))

/*
** asr_s8_x_tied2:
**	asrr	z6\.b, p0/m, z6\.b, z0\.b
**	ret
*/
TEST_DUAL_Z (asr_s8_x_tied2, svint8_t, svuint8_t,
	     z6_res = svasr_s8_x (p0, z0, z6),
	     z6_res = svasr_x (p0, z0, z6))

/*
** asr_s8_x_untied:
** (
**	movprfx	z0, z1
**	asr	z0\.b, p0/m, z0\.b, z6\.b
** |
**	movprfx	z0, z6
**	asrr	z0\.b, p0/m, z0\.b, z1\.b
** )
**	ret
*/
TEST_DUAL_Z (asr_s8_x_untied, svint8_t, svuint8_t,
	     z0 = svasr_s8_x (p0, z1, z6),
	     z0 = svasr_x (p0, z1, z6))

/*
** asr_x0_s8_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s8_x_tied1, svint8_t, uint64_t,
		 z0 = svasr_n_s8_x (p0, z0, x0),
		 z0 = svasr_x (p0, z0, x0))

/*
** asr_x0_s8_x_untied:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s8_x_untied, svint8_t, uint64_t,
		 z0 = svasr_n_s8_x (p0, z1, x0),
		 z0 = svasr_x (p0, z1, x0))

/*
** asr_d4_s8_x_tied1:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.b, z0\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s8_x_tied1, svint8_t, uint64_t,
		 z0 = svasr_n_s8_x (p0, z0, d4),
		 z0 = svasr_x (p0, z0, d4))

/*
** asr_d4_s8_x_untied:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.b, z1\.b, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s8_x_untied, svint8_t, uint64_t,
		 z0 = svasr_n_s8_x (p0, z1, d4),
		 z0 = svasr_x (p0, z1, d4))

/*
** asr_1_s8_x_tied1:
**	asr	z0\.b, z0\.b, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s8_x_tied1, svint8_t,
		z0 = svasr_n_s8_x (p0, z0, 1),
		z0 = svasr_x (p0, z0, 1))

/*
** asr_1_s8_x_untied:
**	asr	z0\.b, z1\.b, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s8_x_untied, svint8_t,
		z0 = svasr_n_s8_x (p0, z1, 1),
		z0 = svasr_x (p0, z1, 1))

/*
** asr_7_s8_x_tied1:
**	asr	z0\.b, z0\.b, #7
**	ret
*/
TEST_UNIFORM_Z (asr_7_s8_x_tied1, svint8_t,
		z0 = svasr_n_s8_x (p0, z0, 7),
		z0 = svasr_x (p0, z0, 7))

/*
** asr_7_s8_x_untied:
**	asr	z0\.b, z1\.b, #7
**	ret
*/
TEST_UNIFORM_Z (asr_7_s8_x_untied, svint8_t,
		z0 = svasr_n_s8_x (p0, z1, 7),
		z0 = svasr_x (p0, z1, 7))

/*
** asr_8_s8_x_tied1:
**	asr	z0\.b, z0\.b, #8
**	ret
*/
TEST_UNIFORM_Z (asr_8_s8_x_tied1, svint8_t,
		z0 = svasr_n_s8_x (p0, z0, 8),
		z0 = svasr_x (p0, z0, 8))

/*
** asr_8_s8_x_untied:
**	asr	z0\.b, z1\.b, #8
**	ret
*/
TEST_UNIFORM_Z (asr_8_s8_x_untied, svint8_t,
		z0 = svasr_n_s8_x (p0, z1, 8),
		z0 = svasr_x (p0, z1, 8))
