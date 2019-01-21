/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** asr_s16_m_tied1:
**	asr	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (asr_s16_m_tied1, svint16_t, svuint16_t,
	     z0 = svasr_s16_m (p0, z0, z6),
	     z0 = svasr_m (p0, z0, z6))

/*
** asr_s16_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	asr	z6\.h, p0/m, z6\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (asr_s16_m_tied2, svint16_t, svuint16_t,
	     z6_res = svasr_s16_m (p0, z0, z6),
	     z6_res = svasr_m (p0, z0, z6))

/*
** asr_s16_m_untied:
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (asr_s16_m_untied, svint16_t, svuint16_t,
	     z0 = svasr_s16_m (p0, z1, z6),
	     z0 = svasr_m (p0, z1, z6))

/*
** asr_x0_s16_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s16_m_tied1, svint16_t, uint64_t,
		 z0 = svasr_n_s16_m (p0, z0, x0),
		 z0 = svasr_m (p0, z0, x0))

/*
** asr_x0_s16_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s16_m_untied, svint16_t, uint64_t,
		 z0 = svasr_n_s16_m (p0, z1, x0),
		 z0 = svasr_m (p0, z1, x0))

/*
** asr_d4_s16_m_tied1:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s16_m_tied1, svint16_t, uint64_t,
		 z0 = svasr_n_s16_m (p0, z0, d4),
		 z0 = svasr_m (p0, z0, d4))

/*
** asr_d4_s16_m_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s16_m_untied, svint16_t, uint64_t,
		 z0 = svasr_n_s16_m (p0, z1, d4),
		 z0 = svasr_m (p0, z1, d4))

/*
** asr_1_s16_m_tied1:
**	asr	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s16_m_tied1, svint16_t,
		z0 = svasr_n_s16_m (p0, z0, 1),
		z0 = svasr_m (p0, z0, 1))

/*
** asr_1_s16_m_untied:
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s16_m_untied, svint16_t,
		z0 = svasr_n_s16_m (p0, z1, 1),
		z0 = svasr_m (p0, z1, 1))

/*
** asr_15_s16_m_tied1:
**	asr	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (asr_15_s16_m_tied1, svint16_t,
		z0 = svasr_n_s16_m (p0, z0, 15),
		z0 = svasr_m (p0, z0, 15))

/*
** asr_15_s16_m_untied:
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (asr_15_s16_m_untied, svint16_t,
		z0 = svasr_n_s16_m (p0, z1, 15),
		z0 = svasr_m (p0, z1, 15))

/*
** asr_16_s16_m_tied1:
**	asr	z0\.h, p0/m, z0\.h, #16
**	ret
*/
TEST_UNIFORM_Z (asr_16_s16_m_tied1, svint16_t,
		z0 = svasr_n_s16_m (p0, z0, 16),
		z0 = svasr_m (p0, z0, 16))

/*
** asr_16_s16_m_untied:
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, #16
**	ret
*/
TEST_UNIFORM_Z (asr_16_s16_m_untied, svint16_t,
		z0 = svasr_n_s16_m (p0, z1, 16),
		z0 = svasr_m (p0, z1, 16))

/*
** asr_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	asr	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (asr_s16_z_tied1, svint16_t, svuint16_t,
	     z0 = svasr_s16_z (p0, z0, z6),
	     z0 = svasr_z (p0, z0, z6))

/*
** asr_s16_z_tied2:
**	movprfx	z6\.h, p0/z, z6\.h
**	asrr	z6\.h, p0/m, z6\.h, z0\.h
**	ret
*/
TEST_DUAL_Z (asr_s16_z_tied2, svint16_t, svuint16_t,
	     z6_res = svasr_s16_z (p0, z0, z6),
	     z6_res = svasr_z (p0, z0, z6))

/*
** asr_s16_z_untied:
** (
**	movprfx	z0\.h, p0/z, z1\.h
**	asr	z0\.h, p0/m, z0\.h, z6\.h
** |
**	movprfx	z0\.h, p0/z, z6\.h
**	asrr	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_DUAL_Z (asr_s16_z_untied, svint16_t, svuint16_t,
	     z0 = svasr_s16_z (p0, z1, z6),
	     z0 = svasr_z (p0, z1, z6))

/*
** asr_x0_s16_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.h, p0/z, z0\.h
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s16_z_tied1, svint16_t, uint64_t,
		 z0 = svasr_n_s16_z (p0, z0, x0),
		 z0 = svasr_z (p0, z0, x0))

/*
** asr_x0_s16_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.h, p0/z, z1\.h
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s16_z_untied, svint16_t, uint64_t,
		 z0 = svasr_n_s16_z (p0, z1, x0),
		 z0 = svasr_z (p0, z1, x0))

/*
** asr_d4_s16_z_tied1:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.h, p0/z, z0\.h
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s16_z_tied1, svint16_t, uint64_t,
		 z0 = svasr_n_s16_z (p0, z0, d4),
		 z0 = svasr_z (p0, z0, d4))

/*
** asr_d4_s16_z_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.h, p0/z, z1\.h
**	asr	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s16_z_untied, svint16_t, uint64_t,
		 z0 = svasr_n_s16_z (p0, z1, d4),
		 z0 = svasr_z (p0, z1, d4))

/*
** asr_1_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	asr	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s16_z_tied1, svint16_t,
		z0 = svasr_n_s16_z (p0, z0, 1),
		z0 = svasr_z (p0, z0, 1))

/*
** asr_1_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	asr	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s16_z_untied, svint16_t,
		z0 = svasr_n_s16_z (p0, z1, 1),
		z0 = svasr_z (p0, z1, 1))

/*
** asr_15_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	asr	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (asr_15_s16_z_tied1, svint16_t,
		z0 = svasr_n_s16_z (p0, z0, 15),
		z0 = svasr_z (p0, z0, 15))

/*
** asr_15_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	asr	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (asr_15_s16_z_untied, svint16_t,
		z0 = svasr_n_s16_z (p0, z1, 15),
		z0 = svasr_z (p0, z1, 15))

/*
** asr_16_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	asr	z0\.h, p0/m, z0\.h, #16
**	ret
*/
TEST_UNIFORM_Z (asr_16_s16_z_tied1, svint16_t,
		z0 = svasr_n_s16_z (p0, z0, 16),
		z0 = svasr_z (p0, z0, 16))

/*
** asr_16_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	asr	z0\.h, p0/m, z0\.h, #16
**	ret
*/
TEST_UNIFORM_Z (asr_16_s16_z_untied, svint16_t,
		z0 = svasr_n_s16_z (p0, z1, 16),
		z0 = svasr_z (p0, z1, 16))

/*
** asr_s16_x_tied1:
**	asr	z0\.h, p0/m, z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (asr_s16_x_tied1, svint16_t, svuint16_t,
	     z0 = svasr_s16_x (p0, z0, z6),
	     z0 = svasr_x (p0, z0, z6))

/*
** asr_s16_x_tied2:
**	asrr	z6\.h, p0/m, z6\.h, z0\.h
**	ret
*/
TEST_DUAL_Z (asr_s16_x_tied2, svint16_t, svuint16_t,
	     z6_res = svasr_s16_x (p0, z0, z6),
	     z6_res = svasr_x (p0, z0, z6))

/*
** asr_s16_x_untied:
** (
**	movprfx	z0, z1
**	asr	z0\.h, p0/m, z0\.h, z6\.h
** |
**	movprfx	z0, z6
**	asrr	z0\.h, p0/m, z0\.h, z1\.h
** )
**	ret
*/
TEST_DUAL_Z (asr_s16_x_untied, svint16_t, svuint16_t,
	     z0 = svasr_s16_x (p0, z1, z6),
	     z0 = svasr_x (p0, z1, z6))

/*
** asr_x0_s16_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.h, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s16_x_tied1, svint16_t, uint64_t,
		 z0 = svasr_n_s16_x (p0, z0, x0),
		 z0 = svasr_x (p0, z0, x0))

/*
** asr_x0_s16_x_untied:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.h, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s16_x_untied, svint16_t, uint64_t,
		 z0 = svasr_n_s16_x (p0, z1, x0),
		 z0 = svasr_x (p0, z1, x0))

/*
** asr_d4_s16_x_tied1:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.h, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s16_x_tied1, svint16_t, uint64_t,
		 z0 = svasr_n_s16_x (p0, z0, d4),
		 z0 = svasr_x (p0, z0, d4))

/*
** asr_d4_s16_x_untied:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.h, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s16_x_untied, svint16_t, uint64_t,
		 z0 = svasr_n_s16_x (p0, z1, d4),
		 z0 = svasr_x (p0, z1, d4))

/*
** asr_1_s16_x_tied1:
**	asr	z0\.h, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s16_x_tied1, svint16_t,
		z0 = svasr_n_s16_x (p0, z0, 1),
		z0 = svasr_x (p0, z0, 1))

/*
** asr_1_s16_x_untied:
**	asr	z0\.h, z1\.h, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s16_x_untied, svint16_t,
		z0 = svasr_n_s16_x (p0, z1, 1),
		z0 = svasr_x (p0, z1, 1))

/*
** asr_15_s16_x_tied1:
**	asr	z0\.h, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (asr_15_s16_x_tied1, svint16_t,
		z0 = svasr_n_s16_x (p0, z0, 15),
		z0 = svasr_x (p0, z0, 15))

/*
** asr_15_s16_x_untied:
**	asr	z0\.h, z1\.h, #15
**	ret
*/
TEST_UNIFORM_Z (asr_15_s16_x_untied, svint16_t,
		z0 = svasr_n_s16_x (p0, z1, 15),
		z0 = svasr_x (p0, z1, 15))

/*
** asr_16_s16_x_tied1:
**	asr	z0\.h, z0\.h, #16
**	ret
*/
TEST_UNIFORM_Z (asr_16_s16_x_tied1, svint16_t,
		z0 = svasr_n_s16_x (p0, z0, 16),
		z0 = svasr_x (p0, z0, 16))

/*
** asr_16_s16_x_untied:
**	asr	z0\.h, z1\.h, #16
**	ret
*/
TEST_UNIFORM_Z (asr_16_s16_x_untied, svint16_t,
		z0 = svasr_n_s16_x (p0, z1, 16),
		z0 = svasr_x (p0, z1, 16))
