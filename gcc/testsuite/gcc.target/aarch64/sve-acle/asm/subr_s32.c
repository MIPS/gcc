/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** subr_s32_m:
**	subr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_m, svint32_t,
		z0 = svsubr_s32_m (p0, z0, z1),
		z0 = svsubr_m (p0, z0, z1))

/*
** subr_s32_m_prfx:
**	movprfx	z0, z1
**	subr	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_m_prfx, svint32_t,
		z0 = svsubr_s32_m (p0, z1, z2),
		z0 = svsubr_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (subr_s32_m_bad_ra, svint32_t,
		z1 = svsubr_s32_m (p0, z0, z1),
		z1 = svsubr_m (p0, z0, z1))

/*
** subr_w0_s32_m:
**	mov	(z[0-9]+\.s), w0
**	subr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s32_m, svint32_t, int32_t,
		 z0 = svsubr_n_s32_m (p0, z0, x0),
		 z0 = svsubr_m (p0, z0, x0))

/*
** subr_w0_s32_m_prfx:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	subr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s32_m_prfx, svint32_t, int32_t,
		 z0 = svsubr_n_s32_m (p0, z1, x0),
		 z0 = svsubr_m (p0, z1, x0))

/*
** subr_s0_s32_m:
**	mov	(z[0-9]+\.s), s0
**	subr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_s32_m, svint32_t, int32_t,
		 z1 = svsubr_n_s32_m (p0, z1, d0),
		 z1 = svsubr_m (p0, z1, d0))

/*
** subr_s0_s32_m_prfx:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	subr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_s32_m_prfx, svint32_t, int32_t,
		 z1 = svsubr_n_s32_m (p0, z2, d0),
		 z1 = svsubr_m (p0, z2, d0))

/*
** subr_1_s32_m:
**	mov	(z[0-9]+\.s), #1
**	subr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s32_m, svint32_t,
		z0 = svsubr_n_s32_m (p0, z0, 1),
		z0 = svsubr_m (p0, z0, 1))

/*
** subr_1_s32_m_prfx:
**	mov	(z[0-9]+\.s), #1
**	movprfx	z0, z1
**	subr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s32_m_prfx, svint32_t,
		z0 = svsubr_n_s32_m (p0, z1, 1),
		z0 = svsubr_m (p0, z1, 1))

/*
** subr_m2_s32_m:
**	mov	(z[0-9]+\.s), #-2
**	subr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (subr_m2_s32_m, svint32_t,
		z0 = svsubr_n_s32_m (p0, z0, -2),
		z0 = svsubr_m (p0, z0, -2))

/*
** subr_s32_z_tied:
**	movprfx	z0\.s, p0/z, z0\.s
**	subr	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_z_tied, svint32_t,
		z0 = svsubr_s32_z (p0, z0, z1),
		z0 = svsubr_z (p0, z0, z1))

/*
** subr_s32_z_rev:
**	movprfx	z1\.s, p0/z, z1\.s
**	sub	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_z_rev, svint32_t,
		z1 = svsubr_s32_z (p0, z0, z1),
		z1 = svsubr_z (p0, z0, z1))

/*
** subr_s32_z_untied:
**	movprfx	z0\.s, p0/z, z2\.s
**	sub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_z_untied, svint32_t,
		z0 = svsubr_s32_z (p0, z1, z2),
		z0 = svsubr_z (p0, z1, z2))

/*
** subr_w0_s32_z_tied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	subr	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s32_z_tied, svint32_t, int32_t,
		 z0 = svsubr_n_s32_z (p0, z0, x0),
		 z0 = svsubr_z (p0, z0, x0))

/*
** subr_w0_s32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, \1
**	sub	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s32_z_untied, svint32_t, int32_t,
		 z0 = svsubr_n_s32_z (p0, z1, x0),
		 z0 = svsubr_z (p0, z1, x0))

/*
** subr_s0_s32_z_tied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	subr	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_s32_z_tied, svint32_t, int32_t,
		 z1 = svsubr_n_s32_z (p0, z1, d0),
		 z1 = svsubr_z (p0, z1, d0))

/*
** subr_s0_s32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, \1
**	sub	z1\.s, p0/m, z1\.s, z2\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_s32_z_untied, svint32_t, int32_t,
		 z1 = svsubr_n_s32_z (p0, z2, d0),
		 z1 = svsubr_z (p0, z2, d0))

/*
** subr_s32_x_z0:
**	sub	z0\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_x_z0, svint32_t,
		z0 = svsubr_s32_x (p0, z0, z1),
		z0 = svsubr_x (p0, z0, z1))

/*
** subr_s32_x_z1:
**	sub	z1\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_x_z1, svint32_t,
		z1 = svsubr_s32_x (p0, z0, z1),
		z1 = svsubr_x (p0, z0, z1))

/*
** subr_s32_x_z2:
**	sub	z2\.s, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_s32_x_z2, svint32_t,
		z2 = svsubr_s32_x (p0, z0, z1),
		z2 = svsubr_x (p0, z0, z1))

/*
** subr_w0_s32_x_z0:
**	mov	(z[0-9]+\.s), w0
**	sub	z0\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s32_x_z0, svint32_t, int32_t,
		 z0 = svsubr_n_s32_x (p0, z0, x0),
		 z0 = svsubr_x (p0, z0, x0))

/*
** subr_w0_s32_x_z1:
**	mov	(z[0-9]+\.s), w0
**	sub	z1\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_w0_s32_x_z1, svint32_t, int32_t,
		 z1 = svsubr_n_s32_x (p0, z0, x0),
		 z1 = svsubr_x (p0, z0, x0))

/*
** subr_s0_s32_x_z1:
**	mov	(z[0-9]+\.s), s0
**	sub	z1\.s, \1, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_s32_x_z1, svint32_t, int32_t,
		 z1 = svsubr_n_s32_x (p0, z1, d0),
		 z1 = svsubr_x (p0, z1, d0))

/*
** subr_s0_s32_x_z2:
**	mov	(z[0-9]+\.s), s0
**	sub	z2\.s, \1, z1\.s
**	ret
*/
TEST_UNIFORM_ZS (subr_s0_s32_x_z2, svint32_t, int32_t,
		 z2 = svsubr_n_s32_x (p0, z1, d0),
		 z2 = svsubr_x (p0, z1, d0))

/*
** subr_1_s32_x:
**	subr	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 1),
		z0 = svsubr_x (p0, z0, 1))

/*
** subr_1_s32_x_prfx:
**	movprfx	z0, z1
**	subr	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (subr_1_s32_x_prfx, svint32_t,
		z0 = svsubr_n_s32_x (p0, z1, 1),
		z0 = svsubr_x (p0, z1, 1))

/*
** subr_127_s32_x:
**	subr	z0\.s, z0\.s, #127
**	ret
*/
TEST_UNIFORM_Z (subr_127_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 127),
		z0 = svsubr_x (p0, z0, 127))

/*
** subr_128_s32_x:
**	subr	z0\.s, z0\.s, #128
**	ret
*/
TEST_UNIFORM_Z (subr_128_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 128),
		z0 = svsubr_x (p0, z0, 128))

/*
** subr_255_s32_x:
**	subr	z0\.s, z0\.s, #255
**	ret
*/
TEST_UNIFORM_Z (subr_255_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 255),
		z0 = svsubr_x (p0, z0, 255))

/*
** subr_256_s32_x:
**	subr	z0\.s, z0\.s, #256
**	ret
*/
TEST_UNIFORM_Z (subr_256_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 256),
		z0 = svsubr_x (p0, z0, 256))

/*
** subr_511_s32_x:
**	mov	(z[0-9]+\.s), #511
**	sub	z0\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_511_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 511),
		z0 = svsubr_x (p0, z0, 511))

/*
** subr_512_s32_x:
**	subr	z0\.s, z0\.s, #512
**	ret
*/
TEST_UNIFORM_Z (subr_512_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 512),
		z0 = svsubr_x (p0, z0, 512))

/*
** subr_65280_s32_x:
**	subr	z0\.s, z0\.s, #65280
**	ret
*/
TEST_UNIFORM_Z (subr_65280_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 0xff00),
		z0 = svsubr_x (p0, z0, 0xff00))

/*
** subr_65535_s32_x:
**	mov	(z[0-9]+\.s), #65535
**	sub	z0\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_65535_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 65535),
		z0 = svsubr_x (p0, z0, 65535))

/*
** subr_65536_s32_x:
**	mov	(z[0-9]+\.s), #65536
**	sub	z0\.s, \1, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_65536_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, 65536),
		z0 = svsubr_x (p0, z0, 65536))

/*
** subr_m1_s32_x:
**	mov	(z[0-9]+)\.b, #-1
**	sub	z0\.s, \1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (subr_m1_s32_x, svint32_t,
		z0 = svsubr_n_s32_x (p0, z0, -1),
		z0 = svsubr_x (p0, z0, -1))

/*
** subr_m1_s32_x_untied:
**	mov	(z[0-9]+)\.b, #-1
**	sub	z0\.s, \1\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (subr_m1_s32_x_untied, svint32_t,
		z0 = svsubr_n_s32_x (p0, z1, -1),
		z0 = svsubr_x (p0, z1, -1))
