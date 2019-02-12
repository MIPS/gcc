/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_m_tied1, svint16_t,
		z0 = svlsl_s16_m (p0, z0, z1),
		z0 = svlsl_m (p0, z0, z1))

/*
** lsl_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_m_untied, svint16_t,
		z0 = svlsl_s16_m (p0, z1, z2),
		z0 = svlsl_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (lsl_s16_m_tied2, svint16_t,
		z1 = svlsl_s16_m (p0, z0, z1),
		z1 = svlsl_m (p0, z0, z1))

/*
** lsl_w0_s16_m_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_s16_m_tied1, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_m (p0, z0, x0),
		 z0 = svlsl_m (p0, z0, x0))

/*
** lsl_w0_s16_m_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_s16_m_untied, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_m (p0, z1, x0),
		 z0 = svlsl_m (p0, z1, x0))

/*
** lsl_d0_s16_m_tied1:
**	mov	(z[0-9]+\.d), d0
**	lsl	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_s16_m_tied1, svint16_t, uint64_t,
		 z1 = svlsl_n_s16_m (p0, z1, d0),
		 z1 = svlsl_m (p0, z1, d0))

/*
** lsl_d0_s16_m_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	lsl	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_s16_m_untied, svint16_t, uint64_t,
		 z1 = svlsl_n_s16_m (p0, z2, d0),
		 z1 = svlsl_m (p0, z2, d0))

/* 
** lsl_0_s16_m_tied1:
**	sel	z0\.h, p0, z0\.h, z0\.h
**	ret
*/
TEST_UNIFORM_ZS (lsl_0_s16_m_tied1, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_m (p0, z0, 0),
		 z0 = svlsl_m (p0, z0, 0))

/* 
** lsl_0_s16_m_untied:
**	sel	z0\.h, p0, z1\.h, z1\.h
**	ret
*/
TEST_UNIFORM_ZS (lsl_0_s16_m_untied, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_m (p0, z1, 0),
		 z0 = svlsl_m (p0, z1, 0))

/*
** lsl_1_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_m_tied1, svint16_t,
		z0 = svlsl_n_s16_m (p0, z0, 1),
		z0 = svlsl_m (p0, z0, 1))

/*
** lsl_1_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_m_untied, svint16_t,
		z0 = svlsl_n_s16_m (p0, z1, 1),
		z0 = svlsl_m (p0, z1, 1))

/*
** lsl_m1_s16_m_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	lsl	z0\.h, p0/m, z0\.h, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_s16_m_tied1, svint16_t,
		z0 = svlsl_n_s16_m (p0, z0, -1),
		z0 = svlsl_m (p0, z0, -1))

/*
** lsl_m1_s16_m_untied:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_s16_m_untied, svint16_t,
		z0 = svlsl_n_s16_m (p0, z1, -1),
		z0 = svlsl_m (p0, z1, -1))

/*
** lsl_15_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_m_tied1, svint16_t,
		z0 = svlsl_n_s16_m (p0, z0, 15),
		z0 = svlsl_m (p0, z0, 15))

/*
** lsl_15_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_m_untied, svint16_t,
		z0 = svlsl_n_s16_m (p0, z1, 15),
		z0 = svlsl_m (p0, z1, 15))

/*
** lsl_16_s16_m_tied1:
**	mov	(z[0-9]+\.d), #16
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_m_tied1, svint16_t,
		z0 = svlsl_n_s16_m (p0, z0, 16),
		z0 = svlsl_m (p0, z0, 16))

/*
** lsl_16_s16_m_untied:
**	mov	(z[0-9]+\.d), #16
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_m_untied, svint16_t,
		z0 = svlsl_n_s16_m (p0, z1, 16),
		z0 = svlsl_m (p0, z1, 16))

/*
** lsl_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_z_tied1, svint16_t,
		z0 = svlsl_s16_z (p0, z0, z1),
		z0 = svlsl_z (p0, z0, z1))

/*
** lsl_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_z_untied, svint16_t,
		z0 = svlsl_s16_z (p0, z1, z2),
		z0 = svlsl_z (p0, z1, z2))

/*
** lsl_s16_z_tied2:
**	movprfx	z0\.h, p0/z, z0\.h
**	lslr	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_z_tied2, svint16_t,
		z0 = svlsl_s16_z (p0, z1, z0),
		z0 = svlsl_z (p0, z1, z0))

/*
** lsl_w0_s16_z_tied1:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_s16_z_tied1, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_z (p0, z0, x0),
		 z0 = svlsl_z (p0, z0, x0))

/*
** lsl_w0_s16_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_s16_z_untied, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_z (p0, z1, x0),
		 z0 = svlsl_z (p0, z1, x0))

/*
** lsl_d0_s16_z_tied1:
**	mov	(z[0-9d]+\.d), d0
**	movprfx	z1\.h, p0/z, z1\.h
**	lsl	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_s16_z_tied1, svint16_t, uint64_t,
		 z1 = svlsl_n_s16_z (p0, z1, d0),
		 z1 = svlsl_z (p0, z1, d0))

/*
** lsl_d0_s16_z_untied:
**	mov	(z[0-9d]+\.d), d0
**	movprfx	z1\.h, p0/z, z2\.h
**	lsl	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_s16_z_untied, svint16_t, uint64_t,
		 z1 = svlsl_n_s16_z (p0, z2, d0),
		 z1 = svlsl_z (p0, z2, d0))

/*
** lsl_0_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, #0
**	ret
*/
TEST_UNIFORM_Z (lsl_0_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, 0),
		z0 = svlsl_z (p0, z0, 0))

/*
** lsl_0_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, #0
**	ret
*/
TEST_UNIFORM_Z (lsl_0_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, 0),
		z0 = svlsl_z (p0, z1, 0))

/*
** lsl_1_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, 1),
		z0 = svlsl_z (p0, z0, 1))

/*
** lsl_1_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, 1),
		z0 = svlsl_z (p0, z1, 1))

/*
** lsl_m1_s16_z_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, -1),
		z0 = svlsl_z (p0, z0, -1))

/*
** lsl_m1_s16_z_untied:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, -1),
		z0 = svlsl_z (p0, z1, -1))

/*
** lsl_15_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, 15),
		z0 = svlsl_z (p0, z0, 15))

/*
** lsl_15_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, 15),
		z0 = svlsl_z (p0, z1, 15))

/*
** lsl_16_s16_z_tied1:
**	mov	(z[0-9]+\.d), #16
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_z_tied1, svint16_t,
		z0 = svlsl_n_s16_z (p0, z0, 16),
		z0 = svlsl_z (p0, z0, 16))

/*
** lsl_16_s16_z_untied:
**	mov	(z[0-9]+\.d), #16
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_z_untied, svint16_t,
		z0 = svlsl_n_s16_z (p0, z1, 16),
		z0 = svlsl_z (p0, z1, 16))

/*
** lsl_s16_x_tied1:
**	lsl	z0\.h, p0/m, z0\.h, z1\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_x_tied1, svint16_t,
		z0 = svlsl_s16_x (p0, z0, z1),
		z0 = svlsl_x (p0, z0, z1))

/*
** lsl_s16_x_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, z2\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_x_untied, svint16_t,
		z0 = svlsl_s16_m (p0, z1, z2),
		z0 = svlsl_s16_m (p0, z1, z2))

/*
** lsl_s16_x_tied2:
**	lslr	z1\.h, p0/m, z1\.h, z0\.h
**	ret
*/
TEST_UNIFORM_Z (lsl_s16_x_tied2, svint16_t,
		z1 = svlsl_s16_x (p0, z0, z1),
		z1 = svlsl_x (p0, z0, z1))

/*
** lsl_w0_s16_x_tied1:
**	mov	(z[0-9]+\.d), x0
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_s16_x_tied1, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_x (p0, z0, x0),
		 z0 = svlsl_x (p0, z0, x0))

/*
** lsl_w0_s16_x_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_w0_s16_x_untied, svint16_t, uint64_t,
		 z0 = svlsl_n_s16_x (p0, z1, x0),
		 z0 = svlsl_x (p0, z1, x0))

/*
** lsl_d0_s16_x_tied1:
**	mov	(z[0-9]+\.d), d0
**	lsl	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_s16_x_tied1, svint16_t, uint64_t,
		 z1 = svlsl_n_s16_x (p0, z1, d0),
		 z1 = svlsl_x (p0, z1, d0))

/*
** lsl_d0_s16_x_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	lsl	z1\.h, p0/m, z1\.h, \1
**	ret
*/
TEST_UNIFORM_ZS (lsl_d0_s16_x_untied, svint16_t, uint64_t,
		 z1 = svlsl_n_s16_x (p0, z2, d0),
		 z1 = svlsl_x (p0, z2, d0))

/*
** lsl_0_s16_x_tied1:
**	ret
*/
TEST_UNIFORM_Z (lsl_0_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, 0),
		z0 = svlsl_x (p0, z0, 0))

/*
** lsl_0_s16_x_untied:
**	mov	z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_0_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, 0),
		z0 = svlsl_x (p0, z1, 0))

/*
** lsl_1_s16_x_tied1:
**	lsl	z0\.h, z0\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, 1),
		z0 = svlsl_x (p0, z0, 1))

/*
** lsl_1_s16_x_untied:
**	lsl	z0\.h, z1\.h, #1
**	ret
*/
TEST_UNIFORM_Z (lsl_1_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, 1),
		z0 = svlsl_x (p0, z1, 1))

/*
** lsl_m1_s16_x_tied1:
**	mov	(z[0-9]+)\.b, #-1
**	lsl	z0\.h, p0/m, z0\.h, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, -1),
		z0 = svlsl_x (p0, z0, -1))

/*
** lsl_m1_s16_x_untied:
**	mov	(z[0-9]+)\.b, #-1
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1\.d
**	ret
*/
TEST_UNIFORM_Z (lsl_m1_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, -1),
		z0 = svlsl_x (p0, z1, -1))

/*
** lsl_15_s16_x_tied1:
**	lsl	z0\.h, z0\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, 15),
		z0 = svlsl_x (p0, z0, 15))

/*
** lsl_15_s16_x_untied:
**	lsl	z0\.h, z1\.h, #15
**	ret
*/
TEST_UNIFORM_Z (lsl_15_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, 15),
		z0 = svlsl_x (p0, z1, 15))

/*
** lsl_16_s16_x_tied1:
**	mov	(z[0-9]+\.d), #16
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_x_tied1, svint16_t,
		z0 = svlsl_n_s16_x (p0, z0, 16),
		z0 = svlsl_x (p0, z0, 16))

/*
** lsl_16_s16_x_untied:
**	mov	(z[0-9]+\.d), #16
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, \1
**	ret
*/
TEST_UNIFORM_Z (lsl_16_s16_x_untied, svint16_t,
		z0 = svlsl_n_s16_x (p0, z1, 16),
		z0 = svlsl_x (p0, z1, 16))
