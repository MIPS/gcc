/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** asr_s32_m_tied1:
**	asr	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (asr_s32_m_tied1, svint32_t, svuint32_t,
	     z0 = svasr_s32_m (p0, z0, z6),
	     z0 = svasr_m (p0, z0, z6))

/*
** asr_s32_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	asr	z6\.s, p0/m, z6\.s, \1\.s
**	ret
*/
TEST_DUAL_Z (asr_s32_m_tied2, svint32_t, svuint32_t,
	     z6_res = svasr_s32_m (p0, z0, z6),
	     z6_res = svasr_m (p0, z0, z6))

/*
** asr_s32_m_untied:
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (asr_s32_m_untied, svint32_t, svuint32_t,
	     z0 = svasr_s32_m (p0, z1, z6),
	     z0 = svasr_m (p0, z1, z6))

/*
** asr_x0_s32_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s32_m_tied1, svint32_t, uint64_t,
		 z0 = svasr_n_s32_m (p0, z0, x0),
		 z0 = svasr_m (p0, z0, x0))

/*
** asr_x0_s32_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s32_m_untied, svint32_t, uint64_t,
		 z0 = svasr_n_s32_m (p0, z1, x0),
		 z0 = svasr_m (p0, z1, x0))

/*
** asr_d4_s32_m_tied1:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s32_m_tied1, svint32_t, uint64_t,
		 z0 = svasr_n_s32_m (p0, z0, d4),
		 z0 = svasr_m (p0, z0, d4))

/*
** asr_d4_s32_m_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s32_m_untied, svint32_t, uint64_t,
		 z0 = svasr_n_s32_m (p0, z1, d4),
		 z0 = svasr_m (p0, z1, d4))

/*
** asr_1_s32_m_tied1:
**	asr	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s32_m_tied1, svint32_t,
		z0 = svasr_n_s32_m (p0, z0, 1),
		z0 = svasr_m (p0, z0, 1))

/*
** asr_1_s32_m_untied:
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s32_m_untied, svint32_t,
		z0 = svasr_n_s32_m (p0, z1, 1),
		z0 = svasr_m (p0, z1, 1))

/*
** asr_31_s32_m_tied1:
**	asr	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (asr_31_s32_m_tied1, svint32_t,
		z0 = svasr_n_s32_m (p0, z0, 31),
		z0 = svasr_m (p0, z0, 31))

/*
** asr_31_s32_m_untied:
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (asr_31_s32_m_untied, svint32_t,
		z0 = svasr_n_s32_m (p0, z1, 31),
		z0 = svasr_m (p0, z1, 31))

/*
** asr_32_s32_m_tied1:
**	asr	z0\.s, p0/m, z0\.s, #32
**	ret
*/
TEST_UNIFORM_Z (asr_32_s32_m_tied1, svint32_t,
		z0 = svasr_n_s32_m (p0, z0, 32),
		z0 = svasr_m (p0, z0, 32))

/*
** asr_32_s32_m_untied:
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, #32
**	ret
*/
TEST_UNIFORM_Z (asr_32_s32_m_untied, svint32_t,
		z0 = svasr_n_s32_m (p0, z1, 32),
		z0 = svasr_m (p0, z1, 32))

/*
** asr_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	asr	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (asr_s32_z_tied1, svint32_t, svuint32_t,
	     z0 = svasr_s32_z (p0, z0, z6),
	     z0 = svasr_z (p0, z0, z6))

/*
** asr_s32_z_tied2:
**	movprfx	z6\.s, p0/z, z6\.s
**	asrr	z6\.s, p0/m, z6\.s, z0\.s
**	ret
*/
TEST_DUAL_Z (asr_s32_z_tied2, svint32_t, svuint32_t,
	     z6_res = svasr_s32_z (p0, z0, z6),
	     z6_res = svasr_z (p0, z0, z6))

/*
** asr_s32_z_untied:
** (
**	movprfx	z0\.s, p0/z, z1\.s
**	asr	z0\.s, p0/m, z0\.s, z6\.s
** |
**	movprfx	z0\.s, p0/z, z6\.s
**	asrr	z0\.s, p0/m, z0\.s, z1\.s
** )
**	ret
*/
TEST_DUAL_Z (asr_s32_z_untied, svint32_t, svuint32_t,
	     z0 = svasr_s32_z (p0, z1, z6),
	     z0 = svasr_z (p0, z1, z6))

/*
** asr_x0_s32_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.s, p0/z, z0\.s
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s32_z_tied1, svint32_t, uint64_t,
		 z0 = svasr_n_s32_z (p0, z0, x0),
		 z0 = svasr_z (p0, z0, x0))

/*
** asr_x0_s32_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.s, p0/z, z1\.s
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s32_z_untied, svint32_t, uint64_t,
		 z0 = svasr_n_s32_z (p0, z1, x0),
		 z0 = svasr_z (p0, z1, x0))

/*
** asr_d4_s32_z_tied1:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.s, p0/z, z0\.s
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s32_z_tied1, svint32_t, uint64_t,
		 z0 = svasr_n_s32_z (p0, z0, d4),
		 z0 = svasr_z (p0, z0, d4))

/*
** asr_d4_s32_z_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.s, p0/z, z1\.s
**	asr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s32_z_untied, svint32_t, uint64_t,
		 z0 = svasr_n_s32_z (p0, z1, d4),
		 z0 = svasr_z (p0, z1, d4))

/*
** asr_1_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	asr	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s32_z_tied1, svint32_t,
		z0 = svasr_n_s32_z (p0, z0, 1),
		z0 = svasr_z (p0, z0, 1))

/*
** asr_1_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	asr	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s32_z_untied, svint32_t,
		z0 = svasr_n_s32_z (p0, z1, 1),
		z0 = svasr_z (p0, z1, 1))

/*
** asr_31_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	asr	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (asr_31_s32_z_tied1, svint32_t,
		z0 = svasr_n_s32_z (p0, z0, 31),
		z0 = svasr_z (p0, z0, 31))

/*
** asr_31_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	asr	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (asr_31_s32_z_untied, svint32_t,
		z0 = svasr_n_s32_z (p0, z1, 31),
		z0 = svasr_z (p0, z1, 31))

/*
** asr_32_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	asr	z0\.s, p0/m, z0\.s, #32
**	ret
*/
TEST_UNIFORM_Z (asr_32_s32_z_tied1, svint32_t,
		z0 = svasr_n_s32_z (p0, z0, 32),
		z0 = svasr_z (p0, z0, 32))

/*
** asr_32_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	asr	z0\.s, p0/m, z0\.s, #32
**	ret
*/
TEST_UNIFORM_Z (asr_32_s32_z_untied, svint32_t,
		z0 = svasr_n_s32_z (p0, z1, 32),
		z0 = svasr_z (p0, z1, 32))

/*
** asr_s32_x_tied1:
**	asr	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (asr_s32_x_tied1, svint32_t, svuint32_t,
	     z0 = svasr_s32_x (p0, z0, z6),
	     z0 = svasr_x (p0, z0, z6))

/*
** asr_s32_x_tied2:
**	asrr	z6\.s, p0/m, z6\.s, z0\.s
**	ret
*/
TEST_DUAL_Z (asr_s32_x_tied2, svint32_t, svuint32_t,
	     z6_res = svasr_s32_x (p0, z0, z6),
	     z6_res = svasr_x (p0, z0, z6))

/*
** asr_s32_x_untied:
** (
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, z6\.s
** |
**	movprfx	z0, z6
**	asrr	z0\.s, p0/m, z0\.s, z1\.s
** )
**	ret
*/
TEST_DUAL_Z (asr_s32_x_untied, svint32_t, svuint32_t,
	     z0 = svasr_s32_x (p0, z1, z6),
	     z0 = svasr_x (p0, z1, z6))

/*
** asr_x0_s32_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s32_x_tied1, svint32_t, uint64_t,
		 z0 = svasr_n_s32_x (p0, z0, x0),
		 z0 = svasr_x (p0, z0, x0))

/*
** asr_x0_s32_x_untied:
**	mov	(z[0-9]+\.d), x0
**	asr	z0\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_x0_s32_x_untied, svint32_t, uint64_t,
		 z0 = svasr_n_s32_x (p0, z1, x0),
		 z0 = svasr_x (p0, z1, x0))

/*
** asr_d4_s32_x_tied1:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s32_x_tied1, svint32_t, uint64_t,
		 z0 = svasr_n_s32_x (p0, z0, d4),
		 z0 = svasr_x (p0, z0, d4))

/*
** asr_d4_s32_x_untied:
**	mov	(z[0-9]+\.d), d4
**	asr	z0\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (asr_d4_s32_x_untied, svint32_t, uint64_t,
		 z0 = svasr_n_s32_x (p0, z1, d4),
		 z0 = svasr_x (p0, z1, d4))

/*
** asr_1_s32_x_tied1:
**	asr	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s32_x_tied1, svint32_t,
		z0 = svasr_n_s32_x (p0, z0, 1),
		z0 = svasr_x (p0, z0, 1))

/*
** asr_1_s32_x_untied:
**	asr	z0\.s, z1\.s, #1
**	ret
*/
TEST_UNIFORM_Z (asr_1_s32_x_untied, svint32_t,
		z0 = svasr_n_s32_x (p0, z1, 1),
		z0 = svasr_x (p0, z1, 1))

/*
** asr_31_s32_x_tied1:
**	asr	z0\.s, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (asr_31_s32_x_tied1, svint32_t,
		z0 = svasr_n_s32_x (p0, z0, 31),
		z0 = svasr_x (p0, z0, 31))

/*
** asr_31_s32_x_untied:
**	asr	z0\.s, z1\.s, #31
**	ret
*/
TEST_UNIFORM_Z (asr_31_s32_x_untied, svint32_t,
		z0 = svasr_n_s32_x (p0, z1, 31),
		z0 = svasr_x (p0, z1, 31))

/*
** asr_32_s32_x_tied1:
**	asr	z0\.s, z0\.s, #32
**	ret
*/
TEST_UNIFORM_Z (asr_32_s32_x_tied1, svint32_t,
		z0 = svasr_n_s32_x (p0, z0, 32),
		z0 = svasr_x (p0, z0, 32))

/*
** asr_32_s32_x_untied:
**	asr	z0\.s, z1\.s, #32
**	ret
*/
TEST_UNIFORM_Z (asr_32_s32_x_untied, svint32_t,
		z0 = svasr_n_s32_x (p0, z1, 32),
		z0 = svasr_x (p0, z1, 32))
