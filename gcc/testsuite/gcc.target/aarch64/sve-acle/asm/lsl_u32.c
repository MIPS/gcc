/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_u32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_m_tied1, svuint32_t,
		z0 = svlsl_u32_m (p0, z0, z1),
		z0 = svlsl_m (p0, z0, z1))

/*
** lsl_u32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_m_untied, svuint32_t,
		z0 = svlsl_u32_m (p0, z1, z2),
		z0 = svlsl_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (lsl_u32_m_tied2, svuint32_t,
		z1 = svlsl_u32_m (p0, z0, z1),
		z1 = svlsl_m (p0, z0, z1))

/*
** lsl_w0_u32_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_u32_m_tied1, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_m (p0, z0, x0),
		 z0 = svlsl_m (p0, z0, x0))

/*
** lsl_w0_u32_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_u32_m_untied, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_m (p0, z1, x0),
		 z0 = svlsl_m (p0, z1, x0))

/*
** lsl_d0_u32_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	lsl	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_u32_m_tied1, svuint32_t, uint64_t,
		 z1 = svlsl_n_u32_m (p0, z1, d0),
		 z1 = svlsl_m (p0, z1, d0))

/*
** lsl_d0_u32_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	lsl	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_u32_m_untied, svuint32_t, uint64_t,
		 z1 = svlsl_n_u32_m (p0, z2, d0),
		 z1 = svlsl_m (p0, z2, d0))

/* 
** lsl_0_u32_m_tied1:
**	sel	z0\.s, p0, z0\.s, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (lsl_0_u32_m_tied1, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_m (p0, z0, 0),
		 z0 = svlsl_m (p0, z0, 0))

/* 
** lsl_0_u32_m_untied:
**	sel	z0\.s, p0, z1\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (lsl_0_u32_m_untied, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_m (p0, z1, 0),
		 z0 = svlsl_m (p0, z1, 0))

/*
** lsl_1_u32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_u32_m_tied1, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z0, 1),
		z0 = svlsl_m (p0, z0, 1))

/*
** lsl_1_u32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_u32_m_untied, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z1, 1),
		z0 = svlsl_m (p0, z1, 1))

/*
** lsl_m1_u32_m_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	lsl	z0\.s, p0/m, z0\.s, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_u32_m_tied1, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z0, -1),
		z0 = svlsl_m (p0, z0, -1))

/*
** lsl_m1_u32_m_untied:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_u32_m_untied, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z1, -1),
		z0 = svlsl_m (p0, z1, -1))

/*
** lsl_31_u32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_u32_m_tied1, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z0, 31),
		z0 = svlsl_m (p0, z0, 31))

/*
** lsl_31_u32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_u32_m_untied, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z1, 31),
		z0 = svlsl_m (p0, z1, 31))

/*
** lsl_32_u32_m_tied1:
**	mov	(z[0-9]+\.d), #32
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_u32_m_tied1, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z0, 32),
		z0 = svlsl_m (p0, z0, 32))

/*
** lsl_32_u32_m_untied:
**	mov	(z[0-9]+\.d), #32
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_u32_m_untied, svuint32_t,
		z0 = svlsl_n_u32_m (p0, z1, 32),
		z0 = svlsl_m (p0, z1, 32))

/*
** lsl_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_z_tied1, svuint32_t,
		z0 = svlsl_u32_z (p0, z0, z1),
		z0 = svlsl_z (p0, z0, z1))

/*
** lsl_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_z_untied, svuint32_t,
		z0 = svlsl_u32_z (p0, z1, z2),
		z0 = svlsl_z (p0, z1, z2))

/*
** lsl_u32_z_tied2:
**	movprfx	z0\.s, p0/z, z0\.s
**	lslr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_z_tied2, svuint32_t,
		z0 = svlsl_u32_z (p0, z1, z0),
		z0 = svlsl_z (p0, z1, z0))

/*
** lsl_w0_u32_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_u32_z_tied1, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_z (p0, z0, x0),
		 z0 = svlsl_z (p0, z0, x0))

/*
** lsl_w0_u32_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_u32_z_untied, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_z (p0, z1, x0),
		 z0 = svlsl_z (p0, z1, x0))

/*
** lsl_d0_u32_z_tied1:
**	mov	(z[0-9d]+\.d), d0
**	movprfx	z1\.s, p0/z, z1\.s
**	lsl	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_u32_z_tied1, svuint32_t, uint64_t,
		 z1 = svlsl_n_u32_z (p0, z1, d0),
		 z1 = svlsl_z (p0, z1, d0))

/*
** lsl_d0_u32_z_untied:
**	mov	(z[0-9d]+\.d), d0
**	movprfx	z1\.s, p0/z, z2\.s
**	lsl	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_u32_z_untied, svuint32_t, uint64_t,
		 z1 = svlsl_n_u32_z (p0, z2, d0),
		 z1 = svlsl_z (p0, z2, d0))

/*
** lsl_0_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, #0
**	ret
*/
TEST_UNIFORM_Z (lsl_0_u32_z_tied1, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z0, 0),
		z0 = svlsl_z (p0, z0, 0))

/*
** lsl_0_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, #0
**	ret
*/
TEST_UNIFORM_Z (lsl_0_u32_z_untied, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z1, 0),
		z0 = svlsl_z (p0, z1, 0))

/*
** lsl_1_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_u32_z_tied1, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z0, 1),
		z0 = svlsl_z (p0, z0, 1))

/*
** lsl_1_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_u32_z_untied, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z1, 1),
		z0 = svlsl_z (p0, z1, 1))

/*
** lsl_m1_u32_z_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_u32_z_tied1, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z0, -1),
		z0 = svlsl_z (p0, z0, -1))

/*
** lsl_m1_u32_z_untied:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_u32_z_untied, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z1, -1),
		z0 = svlsl_z (p0, z1, -1))

/*
** lsl_31_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_u32_z_tied1, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z0, 31),
		z0 = svlsl_z (p0, z0, 31))

/*
** lsl_31_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_u32_z_untied, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z1, 31),
		z0 = svlsl_z (p0, z1, 31))

/*
** lsl_32_u32_z_tied1:
**	mov	(z[0-9]+\.d), #32
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_u32_z_tied1, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z0, 32),
		z0 = svlsl_z (p0, z0, 32))

/*
** lsl_32_u32_z_untied:
**	mov	(z[0-9]+\.d), #32
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_u32_z_untied, svuint32_t,
		z0 = svlsl_n_u32_z (p0, z1, 32),
		z0 = svlsl_z (p0, z1, 32))

/*
** lsl_u32_x_tied1:
**	lsl	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_x_tied1, svuint32_t,
		z0 = svlsl_u32_x (p0, z0, z1),
		z0 = svlsl_x (p0, z0, z1))

/*
** lsl_u32_x_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_x_untied, svuint32_t,
		z0 = svlsl_u32_m (p0, z1, z2),
		z0 = svlsl_u32_m (p0, z1, z2))

/*
** lsl_u32_x_tied2:
**	lslr	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (lsl_u32_x_tied2, svuint32_t,
		z1 = svlsl_u32_x (p0, z0, z1),
		z1 = svlsl_x (p0, z0, z1))

/*
** lsl_w0_u32_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_u32_x_tied1, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_x (p0, z0, x0),
		 z0 = svlsl_x (p0, z0, x0))

/*
** lsl_w0_u32_x_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_u32_x_untied, svuint32_t, uint64_t,
		 z0 = svlsl_n_u32_x (p0, z1, x0),
		 z0 = svlsl_x (p0, z1, x0))

/*
** lsl_d0_u32_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	lsl	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_u32_x_tied1, svuint32_t, uint64_t,
		 z1 = svlsl_n_u32_x (p0, z1, d0),
		 z1 = svlsl_x (p0, z1, d0))

/*
** lsl_d0_u32_x_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	lsl	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_u32_x_untied, svuint32_t, uint64_t,
		 z1 = svlsl_n_u32_x (p0, z2, d0),
		 z1 = svlsl_x (p0, z2, d0))

/*
** lsl_0_u32_x_tied1:
**	ret
*/
TEST_UNIFORM_Z (lsl_0_u32_x_tied1, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z0, 0),
		z0 = svlsl_x (p0, z0, 0))

/*
** lsl_0_u32_x_untied:
**	mov	z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_0_u32_x_untied, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z1, 0),
		z0 = svlsl_x (p0, z1, 0))

/*
** lsl_1_u32_x_tied1:
**	lsl	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_u32_x_tied1, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z0, 1),
		z0 = svlsl_x (p0, z0, 1))

/*
** lsl_1_u32_x_untied:
**	lsl	z0\.s, z1\.s, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_u32_x_untied, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z1, 1),
		z0 = svlsl_x (p0, z1, 1))

/*
** lsl_m1_u32_x_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	lsl	z0\.s, p0/m, z0\.s, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_u32_x_tied1, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z0, -1),
		z0 = svlsl_x (p0, z0, -1))

/*
** lsl_m1_u32_x_untied:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_u32_x_untied, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z1, -1),
		z0 = svlsl_x (p0, z1, -1))

/*
** lsl_31_u32_x_tied1:
**	lsl	z0\.s, z0\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_u32_x_tied1, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z0, 31),
		z0 = svlsl_x (p0, z0, 31))

/*
** lsl_31_u32_x_untied:
**	lsl	z0\.s, z1\.s, #31
**	ret
*/
TEST_UNIFORM_Z (lsl_31_u32_x_untied, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z1, 31),
		z0 = svlsl_x (p0, z1, 31))

/*
** lsl_32_u32_x_tied1:
**	mov	(z[0-9]+\.d), #32
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_u32_x_tied1, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z0, 32),
		z0 = svlsl_x (p0, z0, 32))

/*
** lsl_32_u32_x_untied:
**	mov	(z[0-9]+\.d), #32
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_32_u32_x_untied, svuint32_t,
		z0 = svlsl_n_u32_x (p0, z1, 32),
		z0 = svlsl_x (p0, z1, 32))
