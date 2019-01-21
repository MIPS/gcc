/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_s32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (lsl_s32_m_tied1, svint32_t, svuint32_t,
	     z0 = svlsl_s32_m (p0, z0, z6),
	     z0 = svlsl_m (p0, z0, z6))

/*
** lsl_s32_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	lsl	z6\.s, p0/m, z6\.s, \1\.s
**	ret
*/
TEST_DUAL_Z (lsl_s32_m_tied2, svint32_t, svuint32_t,
	     z6_res = svlsl_s32_m (p0, z0, z6),
	     z6_res = svlsl_m (p0, z0, z6))

/*
** lsl_s32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (lsl_s32_m_untied, svint32_t, svuint32_t,
	     z0 = svlsl_s32_m (p0, z1, z6),
	     z0 = svlsl_m (p0, z1, z6))

/*
** lsl_x0_s32_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s32_m_tied1, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_m (p0, z0, x0),
		 z0 = svlsl_m (p0, z0, x0))

/*
** lsl_x0_s32_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s32_m_untied, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_m (p0, z1, x0),
		 z0 = svlsl_m (p0, z1, x0))

/*
** lsl_d4_s32_m_tied1:
**	mov	(z[0-9]+\.d), d4
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s32_m_tied1, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_m (p0, z0, d4),
		 z0 = svlsl_m (p0, z0, d4))

/*
** lsl_d4_s32_m_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s32_m_untied, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_m (p0, z1, d4),
		 z0 = svlsl_m (p0, z1, d4))

/*
** lsl_1_s32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s32_m_tied1, svint32_t,
		z0 = svlsl_n_s32_m (p0, z0, 1),
		z0 = svlsl_m (p0, z0, 1))

/*
** lsl_1_s32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s32_m_untied, svint32_t,
		z0 = svlsl_n_s32_m (p0, z1, 1),
		z0 = svlsl_m (p0, z1, 1))

/*
** lsl_31_s32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_s32_m_tied1, svint32_t,
		z0 = svlsl_n_s32_m (p0, z0, 31),
		z0 = svlsl_m (p0, z0, 31))

/*
** lsl_31_s32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_s32_m_untied, svint32_t,
		z0 = svlsl_n_s32_m (p0, z1, 31),
		z0 = svlsl_m (p0, z1, 31))

/*
** lsl_32_s32_m_tied1:
**	mov	(z[0-9]+\.d), #32
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_s32_m_tied1, svint32_t,
		z0 = svlsl_n_s32_m (p0, z0, 32),
		z0 = svlsl_m (p0, z0, 32))

/*
** lsl_32_s32_m_untied:
**	mov	(z[0-9]+\.d), #32
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_s32_m_untied, svint32_t,
		z0 = svlsl_n_s32_m (p0, z1, 32),
		z0 = svlsl_m (p0, z1, 32))

/*
** lsl_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (lsl_s32_z_tied1, svint32_t, svuint32_t,
	     z0 = svlsl_s32_z (p0, z0, z6),
	     z0 = svlsl_z (p0, z0, z6))

/*
** lsl_s32_z_tied2:
**	movprfx	z6\.s, p0/z, z6\.s
**	lslr	z6\.s, p0/m, z6\.s, z0\.s
**	ret
*/
TEST_DUAL_Z (lsl_s32_z_tied2, svint32_t, svuint32_t,
	     z6_res = svlsl_s32_z (p0, z0, z6),
	     z6_res = svlsl_z (p0, z0, z6))

/*
** lsl_s32_z_untied:
** (
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, z6\.s
** |
**	movprfx	z0\.s, p0/z, z6\.s
**	lslr	z0\.s, p0/m, z0\.s, z1\.s
** )
**	ret
*/
TEST_DUAL_Z (lsl_s32_z_untied, svint32_t, svuint32_t,
	     z0 = svlsl_s32_z (p0, z1, z6),
	     z0 = svlsl_z (p0, z1, z6))

/*
** lsl_x0_s32_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s32_z_tied1, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_z (p0, z0, x0),
		 z0 = svlsl_z (p0, z0, x0))

/*
** lsl_x0_s32_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s32_z_untied, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_z (p0, z1, x0),
		 z0 = svlsl_z (p0, z1, x0))

/*
** lsl_d4_s32_z_tied1:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s32_z_tied1, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_z (p0, z0, d4),
		 z0 = svlsl_z (p0, z0, d4))

/*
** lsl_d4_s32_z_untied:
**	mov	(z[0-9]+\.d), d4
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s32_z_untied, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_z (p0, z1, d4),
		 z0 = svlsl_z (p0, z1, d4))

/*
** lsl_1_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s32_z_tied1, svint32_t,
		z0 = svlsl_n_s32_z (p0, z0, 1),
		z0 = svlsl_z (p0, z0, 1))

/*
** lsl_1_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s32_z_untied, svint32_t,
		z0 = svlsl_n_s32_z (p0, z1, 1),
		z0 = svlsl_z (p0, z1, 1))

/*
** lsl_31_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_s32_z_tied1, svint32_t,
		z0 = svlsl_n_s32_z (p0, z0, 31),
		z0 = svlsl_z (p0, z0, 31))

/*
** lsl_31_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_s32_z_untied, svint32_t,
		z0 = svlsl_n_s32_z (p0, z1, 31),
		z0 = svlsl_z (p0, z1, 31))

/*
** lsl_32_s32_z_tied1:
**	mov	(z[0-9]+\.d), #32
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_s32_z_tied1, svint32_t,
		z0 = svlsl_n_s32_z (p0, z0, 32),
		z0 = svlsl_z (p0, z0, 32))

/*
** lsl_32_s32_z_untied:
**	mov	(z[0-9]+\.d), #32
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_s32_z_untied, svint32_t,
		z0 = svlsl_n_s32_z (p0, z1, 32),
		z0 = svlsl_z (p0, z1, 32))

/*
** lsl_s32_x_tied1:
**	lsl	z0\.s, p0/m, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (lsl_s32_x_tied1, svint32_t, svuint32_t,
	     z0 = svlsl_s32_x (p0, z0, z6),
	     z0 = svlsl_x (p0, z0, z6))

/*
** lsl_s32_x_tied2:
**	lslr	z6\.s, p0/m, z6\.s, z0\.s
**	ret
*/
TEST_DUAL_Z (lsl_s32_x_tied2, svint32_t, svuint32_t,
	     z6_res = svlsl_s32_x (p0, z0, z6),
	     z6_res = svlsl_x (p0, z0, z6))

/*
** lsl_s32_x_untied:
** (
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, z6\.s
** |
**	movprfx	z0, z6
**	lslr	z0\.s, p0/m, z0\.s, z1\.s
** )
**	ret
*/
TEST_DUAL_Z (lsl_s32_x_untied, svint32_t, svuint32_t,
	     z0 = svlsl_s32_x (p0, z1, z6),
	     z0 = svlsl_x (p0, z1, z6))

/*
** lsl_x0_s32_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s32_x_tied1, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_x (p0, z0, x0),
		 z0 = svlsl_x (p0, z0, x0))

/*
** lsl_x0_s32_x_untied:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_x0_s32_x_untied, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_x (p0, z1, x0),
		 z0 = svlsl_x (p0, z1, x0))

/*
** lsl_d4_s32_x_tied1:
**	mov	(z[0-9]+\.d), d4
**	lsl	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s32_x_tied1, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_x (p0, z0, d4),
		 z0 = svlsl_x (p0, z0, d4))

/*
** lsl_d4_s32_x_untied:
**	mov	(z[0-9]+\.d), d4
**	lsl	z0\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d4_s32_x_untied, svint32_t, uint64_t,
		 z0 = svlsl_n_s32_x (p0, z1, d4),
		 z0 = svlsl_x (p0, z1, d4))

/*
** lsl_1_s32_x_tied1:
**	lsl	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s32_x_tied1, svint32_t,
		z0 = svlsl_n_s32_x (p0, z0, 1),
		z0 = svlsl_x (p0, z0, 1))

/*
** lsl_1_s32_x_untied:
**	lsl	z0\.s, z1\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s32_x_untied, svint32_t,
		z0 = svlsl_n_s32_x (p0, z1, 1),
		z0 = svlsl_x (p0, z1, 1))

/*
** lsl_31_s32_x_tied1:
**	lsl	z0\.s, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_s32_x_tied1, svint32_t,
		z0 = svlsl_n_s32_x (p0, z0, 31),
		z0 = svlsl_x (p0, z0, 31))

/*
** lsl_31_s32_x_untied:
**	lsl	z0\.s, z1\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_s32_x_untied, svint32_t,
		z0 = svlsl_n_s32_x (p0, z1, 31),
		z0 = svlsl_x (p0, z1, 31))

/*
** lsl_32_s32_x_tied1:
**	mov	(z[0-9]+\.d), #32
**	lsl	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_s32_x_tied1, svint32_t,
		z0 = svlsl_n_s32_x (p0, z0, 32),
		z0 = svlsl_x (p0, z0, 32))

/*
** lsl_32_s32_x_untied:
**	mov	(z[0-9]+\.d), #32
**	lsl	z0\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_s32_x_untied, svint32_t,
		z0 = svlsl_n_s32_x (p0, z1, 32),
		z0 = svlsl_x (p0, z1, 32))
