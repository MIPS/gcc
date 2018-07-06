/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** add_s64_m:
**	add	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_m, svint64_t,
		z0 = svadd_s64_m (p0, z0, z1),
		z0 = svadd_m (p0, z0, z1))

/*
** add_s64_m_prfx:
**	movprfx	z0, z1
**	add	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_m_prfx, svint64_t,
		z0 = svadd_s64_m (p0, z1, z2),
		z0 = svadd_m (p0, z1, z2))

/* Bad RA choice: no preferred output sequence.  */
TEST_UNIFORM_Z (add_s64_m_bad_ra, svint64_t,
		z1 = svadd_s64_m (p0, z0, z1),
		z1 = svadd_m (p0, z0, z1))

/*
** add_x0_s64_m:
**	mov	(z[0-9]+\.d), x0
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_s64_m, svint64_t, int64_t,
		 z0 = svadd_n_s64_m (p0, z0, x0),
		 z0 = svadd_m (p0, z0, x0))

/*
** add_x0_s64_m_prfx:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0, z1
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_s64_m_prfx, svint64_t, int64_t,
		 z0 = svadd_n_s64_m (p0, z1, x0),
		 z0 = svadd_m (p0, z1, x0))

/*
** add_d0_s64_m:
**	mov	(z[0-9]+\.d), d0
**	add	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_s64_m, svint64_t, int64_t,
		 z1 = svadd_n_s64_m (p0, z1, d0),
		 z1 = svadd_m (p0, z1, d0))

/*
** add_d0_s64_m_prfx:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1, z2
**	add	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_s64_m_prfx, svint64_t, int64_t,
		 z1 = svadd_n_s64_m (p0, z2, d0),
		 z1 = svadd_m (p0, z2, d0))

/*
** add_1_s64_m:
**	mov	(z[0-9]+\.d), #1
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_1_s64_m, svint64_t,
		z0 = svadd_n_s64_m (p0, z0, 1),
		z0 = svadd_m (p0, z0, 1))

/*
** add_1_s64_m_prfx:
**	mov	(z[0-9]+\.d), #1
**	movprfx	z0, z1
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_1_s64_m_prfx, svint64_t,
		z0 = svadd_n_s64_m (p0, z1, 1),
		z0 = svadd_m (p0, z1, 1))

/*
** add_m2_s64_m:
**	mov	(z[0-9]+\.d), #-2
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m2_s64_m, svint64_t,
		z0 = svadd_n_s64_m (p0, z0, -2),
		z0 = svadd_m (p0, z0, -2))

/*
** add_s64_z_tied:
**	movprfx	z0\.d, p0/z, z0\.d
**	add	z0\.d, p0/m, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_z_tied, svint64_t,
		z0 = svadd_s64_z (p0, z0, z1),
		z0 = svadd_z (p0, z0, z1))

/*
** add_s64_z_rev:
**	movprfx	z1\.d, p0/z, z1\.d
**	add	z1\.d, p0/m, z1\.d, z0\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_z_rev, svint64_t,
		z1 = svadd_s64_z (p0, z0, z1),
		z1 = svadd_z (p0, z0, z1))

/*
** add_s64_z_untied:
**	movprfx	z0\.d, p0/z, z1\.d
**	add	z0\.d, p0/m, z0\.d, z2\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_z_untied, svint64_t,
		z0 = svadd_s64_z (p0, z1, z2),
		z0 = svadd_z (p0, z1, z2))

/*
** add_x0_s64_z_tied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z0\.d
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_s64_z_tied, svint64_t, int64_t,
		 z0 = svadd_n_s64_z (p0, z0, x0),
		 z0 = svadd_z (p0, z0, x0))

/*
** add_x0_s64_z_untied:
**	mov	(z[0-9]+\.d), x0
**	movprfx	z0\.d, p0/z, z1\.d
**	add	z0\.d, p0/m, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_s64_z_untied, svint64_t, int64_t,
		 z0 = svadd_n_s64_z (p0, z1, x0),
		 z0 = svadd_z (p0, z1, x0))

/*
** add_d0_s64_z_tied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z1\.d
**	add	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_s64_z_tied, svint64_t, int64_t,
		 z1 = svadd_n_s64_z (p0, z1, d0),
		 z1 = svadd_z (p0, z1, d0))

/*
** add_d0_s64_z_untied:
**	mov	(z[0-9]+\.d), d0
**	movprfx	z1\.d, p0/z, z2\.d
**	add	z1\.d, p0/m, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_s64_z_untied, svint64_t, int64_t,
		 z1 = svadd_n_s64_z (p0, z2, d0),
		 z1 = svadd_z (p0, z2, d0))

/*
** add_s64_x_z0:
**	add	z0\.d, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_x_z0, svint64_t,
		z0 = svadd_s64_x (p0, z0, z1),
		z0 = svadd_x (p0, z0, z1))

/*
** add_s64_x_z1:
**	add	z1\.d, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_x_z1, svint64_t,
		z1 = svadd_s64_x (p0, z0, z1),
		z1 = svadd_x (p0, z0, z1))

/*
** add_s64_x_z2:
**	add	z2\.d, z0\.d, z1\.d
**	ret
*/
TEST_UNIFORM_Z (add_s64_x_z2, svint64_t,
		z2 = svadd_s64_x (p0, z0, z1),
		z2 = svadd_x (p0, z0, z1))

/*
** add_x0_s64_x_z0:
**	mov	(z[0-9]+\.d), x0
**	add	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_s64_x_z0, svint64_t, int64_t,
		 z0 = svadd_n_s64_x (p0, z0, x0),
		 z0 = svadd_x (p0, z0, x0))

/*
** add_x0_s64_x_z1:
**	mov	(z[0-9]+\.d), x0
**	add	z1\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_x0_s64_x_z1, svint64_t, int64_t,
		 z1 = svadd_n_s64_x (p0, z0, x0),
		 z1 = svadd_x (p0, z0, x0))

/*
** add_d0_s64_x_z1:
**	mov	(z[0-9]+\.d), d0
**	add	z1\.d, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_s64_x_z1, svint64_t, int64_t,
		 z1 = svadd_n_s64_x (p0, z1, d0),
		 z1 = svadd_x (p0, z1, d0))

/*
** add_d0_s64_x_z2:
**	mov	(z[0-9]+\.d), d0
**	add	z2\.d, z1\.d, \1
**	ret
*/
TEST_UNIFORM_ZS (add_d0_s64_x_z2, svint64_t, int64_t,
		 z2 = svadd_n_s64_x (p0, z1, d0),
		 z2 = svadd_x (p0, z1, d0))

/*
** add_1_s64_x:
**	add	z0\.d, z0\.d, #1
**	ret
*/
TEST_UNIFORM_Z (add_1_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 1),
		z0 = svadd_x (p0, z0, 1))

/*
** add_1_s64_x_prfx:
**	movprfx	z0, z1
**	add	z0\.d, z0\.d, #1
**	ret
*/
TEST_UNIFORM_Z (add_1_s64_x_prfx, svint64_t,
		z0 = svadd_n_s64_x (p0, z1, 1),
		z0 = svadd_x (p0, z1, 1))

/*
** add_127_s64_x:
**	add	z0\.d, z0\.d, #127
**	ret
*/
TEST_UNIFORM_Z (add_127_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 127),
		z0 = svadd_x (p0, z0, 127))

/*
** add_128_s64_x:
**	add	z0\.d, z0\.d, #128
**	ret
*/
TEST_UNIFORM_Z (add_128_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 128),
		z0 = svadd_x (p0, z0, 128))

/*
** add_255_s64_x:
**	add	z0\.d, z0\.d, #255
**	ret
*/
TEST_UNIFORM_Z (add_255_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 255),
		z0 = svadd_x (p0, z0, 255))

/*
** add_256_s64_x:
**	add	z0\.d, z0\.d, #256
**	ret
*/
TEST_UNIFORM_Z (add_256_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 256),
		z0 = svadd_x (p0, z0, 256))

/*
** add_511_s64_x:
**	mov	(z[0-9]+\.d), #511
**	add	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_511_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 511),
		z0 = svadd_x (p0, z0, 511))

/*
** add_512_s64_x:
**	add	z0\.d, z0\.d, #512
**	ret
*/
TEST_UNIFORM_Z (add_512_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 512),
		z0 = svadd_x (p0, z0, 512))

/*
** add_65280_s64_x:
**	add	z0\.d, z0\.d, #65280
**	ret
*/
TEST_UNIFORM_Z (add_65280_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 0xff00),
		z0 = svadd_x (p0, z0, 0xff00))

/*
** add_65535_s64_x:
**	mov	(z[0-9]+\.d), #65535
**	add	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_65535_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 65535),
		z0 = svadd_x (p0, z0, 65535))

/*
** add_65536_s64_x:
**	mov	(z[0-9]+\.d), #65536
**	add	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_65536_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, 65536),
		z0 = svadd_x (p0, z0, 65536))

/*
** add_m1_s64_x:
**	sub	z0\.d, z0\.d, #1
**	ret
*/
TEST_UNIFORM_Z (add_m1_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -1),
		z0 = svadd_x (p0, z0, -1))

/*
** add_m127_s64_x:
**	sub	z0\.d, z0\.d, #127
**	ret
*/
TEST_UNIFORM_Z (add_m127_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -127),
		z0 = svadd_x (p0, z0, -127))

/*
** add_m128_s64_x:
**	sub	z0\.d, z0\.d, #128
**	ret
*/
TEST_UNIFORM_Z (add_m128_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -128),
		z0 = svadd_x (p0, z0, -128))

/*
** add_m255_s64_x:
**	sub	z0\.d, z0\.d, #255
**	ret
*/
TEST_UNIFORM_Z (add_m255_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -255),
		z0 = svadd_x (p0, z0, -255))

/*
** add_m256_s64_x:
**	sub	z0\.d, z0\.d, #256
**	ret
*/
TEST_UNIFORM_Z (add_m256_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -256),
		z0 = svadd_x (p0, z0, -256))

/*
** add_m511_s64_x:
**	mov	(z[0-9]+\.d), #(-511|65279)
**	add	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m511_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -511),
		z0 = svadd_x (p0, z0, -511))

/*
** add_m512_s64_x:
**	sub	z0\.d, z0\.d, #512
**	ret
*/
TEST_UNIFORM_Z (add_m512_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -512),
		z0 = svadd_x (p0, z0, -512))

/*
** add_m32768_s64_x:
**	sub	z0\.d, z0\.d, #32768
**	ret
*/
TEST_UNIFORM_Z (add_m32768_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -0x8000),
		z0 = svadd_x (p0, z0, -0x8000))

/*
** add_m65280_s64_x:
**	sub	z0\.d, z0\.d, #65280
**	ret
*/
TEST_UNIFORM_Z (add_m65280_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -0xff00),
		z0 = svadd_x (p0, z0, -0xff00))

/*
** add_m65535_s64_x:
**	mov	(z[0-9]+\.d), #-65535
**	add	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m65535_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -65535),
		z0 = svadd_x (p0, z0, -65535))

/*
** add_m65536_s64_x:
**	mov	(z[0-9]+\.d), #-65536
**	add	z0\.d, z0\.d, \1
**	ret
*/
TEST_UNIFORM_Z (add_m65536_s64_x, svint64_t,
		z0 = svadd_n_s64_x (p0, z0, -65536),
		z0 = svadd_x (p0, z0, -65536))
