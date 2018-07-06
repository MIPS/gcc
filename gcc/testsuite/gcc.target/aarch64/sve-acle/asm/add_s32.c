/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** add_s32_m:
**	add	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_m, svint32_t,
		z0 = svadd_s32_m (p0, z0, z1),
		z0 = svadd_m (p0, z0, z1))

/*
** add_s32_m_prfx:
**	movprfx	z0, z1
**	add	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_m_prfx, svint32_t,
		z0 = svadd_s32_m (p0, z1, z2),
		z0 = svadd_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (add_s32_m_bad_ra, svint32_t,
		z1 = svadd_s32_m (p0, z0, z1),
		z1 = svadd_m (p0, z0, z1))

/*
** add_w0_s32_m:
**	mov	(z[0-9]+\.s), w0
**	add	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_w0_s32_m, svint32_t, int32_t,
		 z0 = svadd_n_s32_m (p0, z0, x0),
		 z0 = svadd_m (p0, z0, x0))

/*
** add_w0_s32_m_prfx:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0, z1
**	add	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_w0_s32_m_prfx, svint32_t, int32_t,
		 z0 = svadd_n_s32_m (p0, z1, x0),
		 z0 = svadd_m (p0, z1, x0))

/*
** add_s0_s32_m:
**	mov	(z[0-9]+\.s), s0
**	add	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_s0_s32_m, svint32_t, int32_t,
		 z1 = svadd_n_s32_m (p0, z1, d0),
		 z1 = svadd_m (p0, z1, d0))

/*
** add_s0_s32_m_prfx:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1, z2
**	add	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_s0_s32_m_prfx, svint32_t, int32_t,
		 z1 = svadd_n_s32_m (p0, z2, d0),
		 z1 = svadd_m (p0, z2, d0))

/*
** add_1_s32_m:
**	mov	(z[0-9]+\.s), #1
**	add	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_1_s32_m, svint32_t,
		z0 = svadd_n_s32_m (p0, z0, 1),
		z0 = svadd_m (p0, z0, 1))

/*
** add_1_s32_m_prfx:
**	mov	(z[0-9]+\.s), #1
**	movprfx	z0, z1
**	add	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_1_s32_m_prfx, svint32_t,
		z0 = svadd_n_s32_m (p0, z1, 1),
		z0 = svadd_m (p0, z1, 1))

/*
** add_m2_s32_m:
**	mov	(z[0-9]+\.s), #-2
**	add	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_m2_s32_m, svint32_t,
		z0 = svadd_n_s32_m (p0, z0, -2),
		z0 = svadd_m (p0, z0, -2))

/*
** add_s32_z_tied:
**	movprfx	z0\.s, p0/z, z0\.s
**	add	z0\.s, p0/m, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_z_tied, svint32_t,
		z0 = svadd_s32_z (p0, z0, z1),
		z0 = svadd_z (p0, z0, z1))

/*
** add_s32_z_rev:
**	movprfx	z1\.s, p0/z, z1\.s
**	add	z1\.s, p0/m, z1\.s, z0\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_z_rev, svint32_t,
		z1 = svadd_s32_z (p0, z0, z1),
		z1 = svadd_z (p0, z0, z1))

/*
** add_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	add	z0\.s, p0/m, z0\.s, z2\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_z_untied, svint32_t,
		z0 = svadd_s32_z (p0, z1, z2),
		z0 = svadd_z (p0, z1, z2))

/*
** add_w0_s32_z_tied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z0\.s
**	add	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_w0_s32_z_tied, svint32_t, int32_t,
		 z0 = svadd_n_s32_z (p0, z0, x0),
		 z0 = svadd_z (p0, z0, x0))

/*
** add_w0_s32_z_untied:
**	mov	(z[0-9]+\.s), w0
**	movprfx	z0\.s, p0/z, z1\.s
**	add	z0\.s, p0/m, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_w0_s32_z_untied, svint32_t, int32_t,
		 z0 = svadd_n_s32_z (p0, z1, x0),
		 z0 = svadd_z (p0, z1, x0))

/*
** add_s0_s32_z_tied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z1\.s
**	add	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_s0_s32_z_tied, svint32_t, int32_t,
		 z1 = svadd_n_s32_z (p0, z1, d0),
		 z1 = svadd_z (p0, z1, d0))

/*
** add_s0_s32_z_untied:
**	mov	(z[0-9]+\.s), s0
**	movprfx	z1\.s, p0/z, z2\.s
**	add	z1\.s, p0/m, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_s0_s32_z_untied, svint32_t, int32_t,
		 z1 = svadd_n_s32_z (p0, z2, d0),
		 z1 = svadd_z (p0, z2, d0))

/*
** add_s32_x_z0:
**	add	z0\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_x_z0, svint32_t,
		z0 = svadd_s32_x (p0, z0, z1),
		z0 = svadd_x (p0, z0, z1))

/*
** add_s32_x_z1:
**	add	z1\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_x_z1, svint32_t,
		z1 = svadd_s32_x (p0, z0, z1),
		z1 = svadd_x (p0, z0, z1))

/*
** add_s32_x_z2:
**	add	z2\.s, z0\.s, z1\.s
**	ret
*/
TEST_UNIFORM_Z (add_s32_x_z2, svint32_t,
		z2 = svadd_s32_x (p0, z0, z1),
		z2 = svadd_x (p0, z0, z1))

/*
** add_w0_s32_x_z0:
**	mov	(z[0-9]+\.s), w0
**	add	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_w0_s32_x_z0, svint32_t, int32_t,
		 z0 = svadd_n_s32_x (p0, z0, x0),
		 z0 = svadd_x (p0, z0, x0))

/*
** add_w0_s32_x_z1:
**	mov	(z[0-9]+\.s), w0
**	add	z1\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_w0_s32_x_z1, svint32_t, int32_t,
		 z1 = svadd_n_s32_x (p0, z0, x0),
		 z1 = svadd_x (p0, z0, x0))

/*
** add_s0_s32_x_z1:
**	mov	(z[0-9]+\.s), s0
**	add	z1\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_s0_s32_x_z1, svint32_t, int32_t,
		 z1 = svadd_n_s32_x (p0, z1, d0),
		 z1 = svadd_x (p0, z1, d0))

/*
** add_s0_s32_x_z2:
**	mov	(z[0-9]+\.s), s0
**	add	z2\.s, z1\.s, \1
**	ret
*/
TEST_UNIFORM_ZS (add_s0_s32_x_z2, svint32_t, int32_t,
		 z2 = svadd_n_s32_x (p0, z1, d0),
		 z2 = svadd_x (p0, z1, d0))

/*
** add_1_s32_x:
**	add	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (add_1_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 1),
		z0 = svadd_x (p0, z0, 1))

/*
** add_1_s32_x_prfx:
**	movprfx	z0, z1
**	add	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (add_1_s32_x_prfx, svint32_t,
		z0 = svadd_n_s32_x (p0, z1, 1),
		z0 = svadd_x (p0, z1, 1))

/*
** add_127_s32_x:
**	add	z0\.s, z0\.s, #127
**	ret
*/
TEST_UNIFORM_Z (add_127_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 127),
		z0 = svadd_x (p0, z0, 127))

/*
** add_128_s32_x:
**	add	z0\.s, z0\.s, #128
**	ret
*/
TEST_UNIFORM_Z (add_128_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 128),
		z0 = svadd_x (p0, z0, 128))

/*
** add_255_s32_x:
**	add	z0\.s, z0\.s, #255
**	ret
*/
TEST_UNIFORM_Z (add_255_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 255),
		z0 = svadd_x (p0, z0, 255))

/*
** add_256_s32_x:
**	add	z0\.s, z0\.s, #256
**	ret
*/
TEST_UNIFORM_Z (add_256_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 256),
		z0 = svadd_x (p0, z0, 256))

/*
** add_511_s32_x:
**	mov	(z[0-9]+\.s), #511
**	add	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_511_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 511),
		z0 = svadd_x (p0, z0, 511))

/*
** add_512_s32_x:
**	add	z0\.s, z0\.s, #512
**	ret
*/
TEST_UNIFORM_Z (add_512_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 512),
		z0 = svadd_x (p0, z0, 512))

/*
** add_65280_s32_x:
**	add	z0\.s, z0\.s, #65280
**	ret
*/
TEST_UNIFORM_Z (add_65280_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 0xff00),
		z0 = svadd_x (p0, z0, 0xff00))

/*
** add_65535_s32_x:
**	mov	(z[0-9]+\.s), #65535
**	add	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_65535_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 65535),
		z0 = svadd_x (p0, z0, 65535))

/*
** add_65536_s32_x:
**	mov	(z[0-9]+\.s), #65536
**	add	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_65536_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, 65536),
		z0 = svadd_x (p0, z0, 65536))

/*
** add_m1_s32_x:
**	sub	z0\.s, z0\.s, #1
**	ret
*/
TEST_UNIFORM_Z (add_m1_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -1),
		z0 = svadd_x (p0, z0, -1))

/*
** add_m127_s32_x:
**	sub	z0\.s, z0\.s, #127
**	ret
*/
TEST_UNIFORM_Z (add_m127_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -127),
		z0 = svadd_x (p0, z0, -127))

/*
** add_m128_s32_x:
**	sub	z0\.s, z0\.s, #128
**	ret
*/
TEST_UNIFORM_Z (add_m128_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -128),
		z0 = svadd_x (p0, z0, -128))

/*
** add_m255_s32_x:
**	sub	z0\.s, z0\.s, #255
**	ret
*/
TEST_UNIFORM_Z (add_m255_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -255),
		z0 = svadd_x (p0, z0, -255))

/*
** add_m256_s32_x:
**	sub	z0\.s, z0\.s, #256
**	ret
*/
TEST_UNIFORM_Z (add_m256_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -256),
		z0 = svadd_x (p0, z0, -256))

/*
** add_m511_s32_x:
**	mov	(z[0-9]+\.s), #(-511|65279)
**	add	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_m511_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -511),
		z0 = svadd_x (p0, z0, -511))

/*
** add_m512_s32_x:
**	sub	z0\.s, z0\.s, #512
**	ret
*/
TEST_UNIFORM_Z (add_m512_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -512),
		z0 = svadd_x (p0, z0, -512))

/*
** add_m32768_s32_x:
**	sub	z0\.s, z0\.s, #32768
**	ret
*/
TEST_UNIFORM_Z (add_m32768_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -0x8000),
		z0 = svadd_x (p0, z0, -0x8000))

/*
** add_m65280_s32_x:
**	sub	z0\.s, z0\.s, #65280
**	ret
*/
TEST_UNIFORM_Z (add_m65280_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -0xff00),
		z0 = svadd_x (p0, z0, -0xff00))

/*
** add_m65535_s32_x:
**	mov	(z[0-9]+\.s), #-65535
**	add	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_m65535_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -65535),
		z0 = svadd_x (p0, z0, -65535))

/*
** add_m65536_s32_x:
**	mov	(z[0-9]+\.s), #-65536
**	add	z0\.s, z0\.s, \1
**	ret
*/
TEST_UNIFORM_Z (add_m65536_s32_x, svint32_t,
		z0 = svadd_n_s32_x (p0, z0, -65536),
		z0 = svadd_x (p0, z0, -65536))
