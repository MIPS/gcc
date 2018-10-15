/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dup_1_f32:
**	fmov	z0\.s, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f32, svfloat32_t,
		z0 = svdup_n_f32 (1),
		z0 = svdup_f32 (1))

/*
** dup_0_f32:
**	mov	z0\.s, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f32, svfloat32_t,
		z0 = svdup_n_f32 (0),
		z0 = svdup_f32 (0))

/*
** dup_8_f32:
**	fmov	z0\.s, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f32, svfloat32_t,
		z0 = svdup_n_f32 (8),
		z0 = svdup_f32 (8))

/*
** dup_512_f32:
**	movi	v([0-9]+).2s, 0x44, lsl 24
**	mov	z0\.s, s\1
**	ret
*/
TEST_UNIFORM_Z (dup_512_f32, svfloat32_t,
		z0 = svdup_n_f32 (512),
		z0 = svdup_f32 (512))

/*
** dup_513_f32:
**	mov	(w[0-7]+), 16384
**	movk	\1, 0x4400, lsl 16
**	fmov	(s[0-7]+), \1
**	mov	z0\.s, \2
**	ret
*/
TEST_UNIFORM_Z (dup_513_f32, svfloat32_t,
		z0 = svdup_n_f32 (513),
		z0 = svdup_f32 (513))

/*
** dup_w0_f32:
**	mov	z1\.s, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f32, svfloat32_t, float,
		z1 = svdup_n_f32 (x0),
		z1 = svdup_f32 (x0))

/*
** dup_s0_f32:
**	mov	z1\.s, s0
**	ret
*/
TEST_UNIFORM_ZS (dup_s0_f32, svfloat32_t, float,
		z1 = svdup_n_f32 (d0),
		z1 = svdup_f32 (d0))

/*
** dup_1_f32_m:
**	fmov	z0\.s, p0/m, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f32_m, svfloat32_t,
		z0 = svdup_n_f32_m (z0, p0, 1),
		z0 = svdup_f32_m (z0, p0, 1))

/*
** dup_0_f32_m:
**	mov	z0\.s, p0/m, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f32_m, svfloat32_t,
		z0 = svdup_n_f32_m (z0, p0, 0),
		z0 = svdup_f32_m (z0, p0, 0))

/*
** dup_8_f32_m:
**	fmov	z0\.s, p0/m, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f32_m, svfloat32_t,
		z0 = svdup_n_f32_m (z0, p0, 8),
		z0 = svdup_f32_m (z0, p0, 8))

/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_512_f32_m, svfloat32_t,
		z0 = svdup_n_f32_m (z0, p0, 512),
		z0 = svdup_f32_m (z0, p0, 512))


/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_513_f32_m, svfloat32_t,
		z0 = svdup_n_f32_m (z0, p0, 513),
		z0 = svdup_f32_m (z0, p0, 513))

/*
** dup_w0_f32_m:
**	movprfx	z1, z0
**	mov	z1\.s, p0/m, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f32_m, svfloat32_t, float,
		z1 = svdup_n_f32_m (z0, p0, x0),
		z1 = svdup_f32_m (z0, p0, x0))

/*
** dup_s0_f32_m:
**	movprfx	z1, z0
**	mov	z1\.s, p0/m, s0
**	ret
*/
TEST_UNIFORM_ZS (dup_s0_f32_m, svfloat32_t, float,
		z1 = svdup_n_f32_m (z0, p0, d0),
		z1 = svdup_f32_m (z0, p0, d0))

/*
** dup_1_f32_z:
**	movprfx	z0\.s, p0/z, z0\.s
**	fmov	z0\.s, p0/m, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f32_z, svfloat32_t,
		z0 = svdup_n_f32_z (p0, 1),
		z0 = svdup_f32_z (p0, 1))

/*
** dup_0_f32_z:
**	mov	z0\.s, p0/z, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f32_z, svfloat32_t,
		z0 = svdup_n_f32_z (p0, 0),
		z0 = svdup_f32_z (p0, 0))

/*
** dup_8_f32_z:
**	movprfx	z0\.s, p0/z, z0\.s
**	fmov	z0\.s, p0/m, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f32_z, svfloat32_t,
		z0 = svdup_n_f32_z (p0, 8),
		z0 = svdup_f32_z (p0, 8))

/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_512_f32_z, svfloat32_t,
		z0 = svdup_n_f32_z (p0, 512),
		z0 = svdup_f32_z (p0, 512))

/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_513_f32_z, svfloat32_t,
		z0 = svdup_n_f32_z (p0, 513),
		z0 = svdup_f32_z (p0, 513))

/*
** dup_w0_f32_z:
**	movprfx	z1\.s, p0/z, z1\.s
**	mov	z1\.s, p0/m, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f32_z, svfloat32_t, float,
		z1 = svdup_n_f32_z (p0, x0),
		z1 = svdup_f32_z (p0, x0))

/*
** dup_s0_f32_z:
**	movprfx	z1\.s, p0/z, z1\.s
**	mov	z1\.s, p0/m, s0
**	ret
*/
TEST_UNIFORM_ZS (dup_s0_f32_z, svfloat32_t, float,
		z1 = svdup_n_f32_z (p0, d0),
		z1 = svdup_f32_z (p0, d0))

/*
** dup_1_f32_x:
**	fmov	z0\.s, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f32_x, svfloat32_t,
		z0 = svdup_n_f32_x (p0, 1),
		z0 = svdup_f32_x (p0, 1))

/*
** dup_0_f32_x:
**	mov	z0\.s, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f32_x, svfloat32_t,
		z0 = svdup_n_f32_x (p0, 0),
		z0 = svdup_f32_x (p0, 0))

/*
** dup_8_f32_x:
**	fmov	z0\.s, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f32_x, svfloat32_t,
		z0 = svdup_n_f32_x (p0, 8),
		z0 = svdup_f32_x (p0, 8))

/*
** dup_512_f32_x:
**	movi	v([0-9]+)\.2s, 0x44, lsl 24
**	mov	z0\.s, s\1
**	ret
*/
TEST_UNIFORM_Z (dup_512_f32_x, svfloat32_t,
		z0 = svdup_n_f32_x (p0, 512),
		z0 = svdup_f32_x (p0, 512))

/*
** dup_513_f32_x:
**	mov	(w[0-7]+), 16384
**	movk	\1, 0x4400, lsl 16
**	fmov	(s[0-7]+), \1
**	mov	z0\.s, \2
**	ret
*/
TEST_UNIFORM_Z (dup_513_f32_x, svfloat32_t,
		z0 = svdup_n_f32_x (p0, 513),
		z0 = svdup_f32_x (p0, 513))

/*
** dup_w0_f32_x:
**	mov	z1\.s, w0
**	ret
*/
TEST_UNIFORM_ZS (dup_w0_f32_x, svfloat32_t, float,
		z1 = svdup_n_f32_x (p0, x0),
		z1 = svdup_f32_x (p0, x0))

/*
** dup_s0_f32_x:
**	mov	z1\.s, s0
**	ret
*/
TEST_UNIFORM_ZS (dup_s0_f32_x, svfloat32_t, float,
		z1 = svdup_n_f32_x (p0, d0),
		z1 = svdup_f32_x (p0, d0))

