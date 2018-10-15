/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dup_1_f16:
**	fmov	z0\.h, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f16, svfloat16_t,
		z0 = svdup_n_f16 (1),
		z0 = svdup_f16 (1))

/*
** dup_0_f16:
**	mov	z0\.h, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f16, svfloat16_t,
		z0 = svdup_n_f16 (0),
		z0 = svdup_f16 (0))

/*
** dup_8_f16:
**	fmov	z0\.h, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f16, svfloat16_t,
		z0 = svdup_n_f16 (8),
		z0 = svdup_f16 (8))

/*
** dup_512_f16:
**	mov	z0\.h, #24576
**	ret
*/
TEST_UNIFORM_Z (dup_512_f16, svfloat16_t,
		z0 = svdup_n_f16 (512),
		z0 = svdup_f16 (512))

/*
** dup_513_f16:
**	mov	(w[0-7]+), 24578
**	fmov	(h[0-7]+), \1
**	mov	z0\.h, \2
**	ret
*/
TEST_UNIFORM_Z (dup_513_f16, svfloat16_t,
		z0 = svdup_n_f16 (513),
		z0 = svdup_f16 (513))

/*
** dup_w0_f16:
**	mov	z1\.h, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f16, svfloat16_t, __fp16,
		z1 = svdup_n_f16 (x0),
		z1 = svdup_f16 (x0))

/*
** dup_h0_f16:
**	mov	z1\.h, h0
**	ret
*/
TEST_UNIFORM_ZS (dup_h0_f16, svfloat16_t, __fp16,
		z1 = svdup_n_f16 (d0),
		z1 = svdup_f16 (d0))

/*
** dup_1_f16_m:
**	fmov	z0\.h, p0/m, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f16_m, svfloat16_t,
		z0 = svdup_n_f16_m (z0, p0, 1),
		z0 = svdup_f16_m (z0, p0, 1))

/*
** dup_0_f16_m:
**	mov	z0\.h, p0/m, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f16_m, svfloat16_t,
		z0 = svdup_n_f16_m (z0, p0, 0),
		z0 = svdup_f16_m (z0, p0, 0))

/*
** dup_8_f16_m:
**	fmov	z0\.h, p0/m, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f16_m, svfloat16_t,
		z0 = svdup_n_f16_m (z0, p0, 8),
		z0 = svdup_f16_m (z0, p0, 8))

/*
** dup_512_f16_m:
**	mov	z0\.h, p0/m, #24576
**	ret
*/
TEST_UNIFORM_Z (dup_512_f16_m, svfloat16_t,
		z0 = svdup_n_f16_m (z0, p0, 512),
		z0 = svdup_f16_m (z0, p0, 512))


/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_513_f16_m, svfloat16_t,
		z0 = svdup_n_f16_m (z0, p0, 513),
		z0 = svdup_f16_m (z0, p0, 513))

/*
** dup_w0_f16_m:
**	movprfx	z1, z0
**	mov	z1\.h, p0/m, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f16_m, svfloat16_t, __fp16,
		z1 = svdup_n_f16_m (z0, p0, x0),
		z1 = svdup_f16_m (z0, p0, x0))

/*
** dup_h0_f16_m:
**	movprfx	z1, z0
**	mov	z1\.h, p0/m, h0
**	ret
*/
TEST_UNIFORM_ZS (dup_h0_f16_m, svfloat16_t, __fp16,
		z1 = svdup_n_f16_m (z0, p0, d0),
		z1 = svdup_f16_m (z0, p0, d0))

/*
** dup_1_f16_z:
**	movprfx	z0\.h, p0/z, z0\.h
**	fmov	z0\.h, p0/m, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f16_z, svfloat16_t,
		z0 = svdup_n_f16_z (p0, 1),
		z0 = svdup_f16_z (p0, 1))

/*
** dup_0_f16_z:
**	mov	z0\.h, p0/z, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f16_z, svfloat16_t,
		z0 = svdup_n_f16_z (p0, 0),
		z0 = svdup_f16_z (p0, 0))

/*
** dup_8_f16_z:
**	movprfx	z0\.h, p0/z, z0\.h
**	fmov	z0\.h, p0/m, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f16_z, svfloat16_t,
		z0 = svdup_n_f16_z (p0, 8),
		z0 = svdup_f16_z (p0, 8))

/*
** dup_512_f16_z:
**	mov	z0\.h, p0/z, #24576
**	ret
*/
TEST_UNIFORM_Z (dup_512_f16_z, svfloat16_t,
		z0 = svdup_n_f16_z (p0, 512),
		z0 = svdup_f16_z (p0, 512))

/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_513_f16_z, svfloat16_t,
		z0 = svdup_n_f16_z (p0, 513),
		z0 = svdup_f16_z (p0, 513))

/*
** dup_w0_f16_z:
**	movprfx	z1\.h, p0/z, z1\.h
**	mov	z1\.h, p0/m, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f16_z, svfloat16_t, __fp16,
		z1 = svdup_n_f16_z (p0, x0),
		z1 = svdup_f16_z (p0, x0))

/*
** dup_h0_f16_z:
**	movprfx	z1\.h, p0/z, z1\.h
**	mov	z1\.h, p0/m, h0
**	ret
*/
TEST_UNIFORM_ZS (dup_h0_f16_z, svfloat16_t, __fp16,
		z1 = svdup_n_f16_z (p0, d0),
		z1 = svdup_f16_z (p0, d0))

/*
** dup_1_f16_x:
**	fmov	z0\.h, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f16_x, svfloat16_t,
		z0 = svdup_n_f16_x (p0, 1),
		z0 = svdup_f16_x (p0, 1))

/*
** dup_0_f16_x:
**	mov	z0\.h, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f16_x, svfloat16_t,
		z0 = svdup_n_f16_x (p0, 0),
		z0 = svdup_f16_x (p0, 0))

/*
** dup_8_f16_x:
**	fmov	z0\.h, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f16_x, svfloat16_t,
		z0 = svdup_n_f16_x (p0, 8),
		z0 = svdup_f16_x (p0, 8))

/*
** dup_512_f16_x:
**	mov	z0\.h, #24576
**	ret
*/
TEST_UNIFORM_Z (dup_512_f16_x, svfloat16_t,
		z0 = svdup_n_f16_x (p0, 512),
		z0 = svdup_f16_x (p0, 512))

/*
** dup_513_f16_x:
**	mov	(w[0-7]+), 24578
**	fmov	(h[0-7]+), \1
**	mov	z0\.h, \2
**	ret
*/
TEST_UNIFORM_Z (dup_513_f16_x, svfloat16_t,
		z0 = svdup_n_f16_x (p0, 513),
		z0 = svdup_f16_x (p0, 513))

/*
** dup_w0_f16_x:
**	mov	z1\.h, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f16_x, svfloat16_t, __fp16,
		z1 = svdup_n_f16_x (p0, x0),
		z1 = svdup_f16_x (p0, x0))

/*
** dup_h0_f16_x:
**	mov	z1\.h, h0
**	ret
*/
TEST_UNIFORM_ZS (dup_h0_f16_x, svfloat16_t, __fp16,
		z1 = svdup_n_f16_x (p0, d0),
		z1 = svdup_f16_x (p0, d0))

