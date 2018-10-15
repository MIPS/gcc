/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dup_1_f64:
**	fmov	z0\.d, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f64, svfloat64_t,
		z0 = svdup_n_f64 (1),
		z0 = svdup_f64 (1))

/*
** dup_0_f64:
**	mov	z0\.d, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f64, svfloat64_t,
		z0 = svdup_n_f64 (0),
		z0 = svdup_f64 (0))

/*
** dup_8_f64:
**	fmov	z0\.d, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f64, svfloat64_t,
		z0 = svdup_n_f64 (8),
		z0 = svdup_f64 (8))

/*
** dup_512_f64:
**	mov	(x[0-9]+), 4647714815446351872
**	fmov	(d[0-9]+), \1
**	mov	z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (dup_512_f64, svfloat64_t,
		z0 = svdup_n_f64 (512),
		z0 = svdup_f64 (512))

/*
** dup_513_f64:
**	mov	(x[0-7]+), 8796093022208
**	movk	\1, 0x4080, lsl 48
**	fmov	(d[0-7]+), \1
**	mov	z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (dup_513_f64, svfloat64_t,
		z0 = svdup_n_f64 (513),
		z0 = svdup_f64 (513))

/*
** dup_x0_f64:
**	mov	z1\.d, x0
**	ret
*/
TEST_UNIFORM_ZS (dup_x0_f64, svfloat64_t, double,
		z1 = svdup_n_f64 (x0),
		z1 = svdup_f64 (x0))

/*
** dup_d0_f64:
**	mov	z1\.d, d0
**	ret
*/
TEST_UNIFORM_ZS (dup_d0_f64, svfloat64_t, double,
		z1 = svdup_n_f64 (d0),
		z1 = svdup_f64 (d0))

/*
** dup_1_f64_m:
**	fmov	z0\.d, p0/m, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f64_m, svfloat64_t,
		z0 = svdup_n_f64_m (z0, p0, 1),
		z0 = svdup_f64_m (z0, p0, 1))

/*
** dup_0_f64_m:
**	mov	z0\.d, p0/m, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f64_m, svfloat64_t,
		z0 = svdup_n_f64_m (z0, p0, 0),
		z0 = svdup_f64_m (z0, p0, 0))

/*
** dup_8_f64_m:
**	fmov	z0\.d, p0/m, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f64_m, svfloat64_t,
		z0 = svdup_n_f64_m (z0, p0, 8),
		z0 = svdup_f64_m (z0, p0, 8))

/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_512_f64_m, svfloat64_t,
		z0 = svdup_n_f64_m (z0, p0, 512),
		z0 = svdup_f64_m (z0, p0, 512))


/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_513_f64_m, svfloat64_t,
		z0 = svdup_n_f64_m (z0, p0, 513),
		z0 = svdup_f64_m (z0, p0, 513))

/*
** dup_x0_f64_m:
**	movprfx	z1, z0
**	mov	z1\.d, p0/m, x0
**	ret
*/
TEST_UNIFORM_ZS (dup_x0_f64_m, svfloat64_t, double,
		z1 = svdup_n_f64_m (z0, p0, x0),
		z1 = svdup_f64_m (z0, p0, x0))

/*
** dup_d0_f64_m:
**	movprfx	z1, z0
**	mov	z1\.d, p0/m, d0
**	ret
*/
TEST_UNIFORM_ZS (dup_d0_f64_m, svfloat64_t, double,
		z1 = svdup_n_f64_m (z0, p0, d0),
		z1 = svdup_f64_m (z0, p0, d0))

/*
** dup_1_f64_z:
**	movprfx	z0\.d, p0/z, z0\.d
**	fmov	z0\.d, p0/m, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f64_z, svfloat64_t,
		z0 = svdup_n_f64_z (p0, 1),
		z0 = svdup_f64_z (p0, 1))

/*
** dup_0_f64_z:
**	mov	z0\.d, p0/z, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f64_z, svfloat64_t,
		z0 = svdup_n_f64_z (p0, 0),
		z0 = svdup_f64_z (p0, 0))

/*
** dup_8_f64_z:
**	movprfx	z0\.d, p0/z, z0\.d
**	fmov	z0\.d, p0/m, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f64_z, svfloat64_t,
		z0 = svdup_n_f64_z (p0, 8),
		z0 = svdup_f64_z (p0, 8))

/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_512_f64_z, svfloat64_t,
		z0 = svdup_n_f64_z (p0, 512),
		z0 = svdup_f64_z (p0, 512))

/* TODO: Bad code and needs fixing.  */
TEST_UNIFORM_Z (dup_513_f64_z, svfloat64_t,
		z0 = svdup_n_f64_z (p0, 513),
		z0 = svdup_f64_z (p0, 513))

/*
** dup_x0_f64_z:
**	movprfx	z1\.d, p0/z, z1\.d
**	mov	z1\.d, p0/m, x0
**	ret
*/
TEST_UNIFORM_ZS (dup_x0_f64_z, svfloat64_t, double,
		z1 = svdup_n_f64_z (p0, x0),
		z1 = svdup_f64_z (p0, x0))

/*
** dup_d0_f64_z:
**	movprfx	z1\.d, p0/z, z1\.d
**	mov	z1\.d, p0/m, d0
**	ret
*/
TEST_UNIFORM_ZS (dup_d0_f64_z, svfloat64_t, double,
		z1 = svdup_n_f64_z (p0, d0),
		z1 = svdup_f64_z (p0, d0))

/*
** dup_1_f64_x:
**	fmov	z0\.d, #1.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_1_f64_x, svfloat64_t,
		z0 = svdup_n_f64_x (p0, 1),
		z0 = svdup_f64_x (p0, 1))

/*
** dup_0_f64_x:
**	mov	z0\.d, #0
**	ret
*/
TEST_UNIFORM_Z (dup_0_f64_x, svfloat64_t,
		z0 = svdup_n_f64_x (p0, 0),
		z0 = svdup_f64_x (p0, 0))

/*
** dup_8_f64_x:
**	fmov	z0\.d, #8.0(e\+0)?
**	ret
*/
TEST_UNIFORM_Z (dup_8_f64_x, svfloat64_t,
		z0 = svdup_n_f64_x (p0, 8),
		z0 = svdup_f64_x (p0, 8))

/*
** dup_512_f64_x:
**	mov	(x[0-9]+), 4647714815446351872
**	fmov	(d[0-9]+), \1
**	mov	z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (dup_512_f64_x, svfloat64_t,
		z0 = svdup_n_f64_x (p0, 512),
		z0 = svdup_f64_x (p0, 512))

/*
** dup_513_f64_x:
**	mov	(x[0-7]+), 8796093022208
**	movk	\1, 0x4080, lsl 48
**	fmov	(d[0-7]+), \1
**	mov	z0\.d, \2
**	ret
*/
TEST_UNIFORM_Z (dup_513_f64_x, svfloat64_t,
		z0 = svdup_n_f64_x (p0, 513),
		z0 = svdup_f64_x (p0, 513))

/*
** dup_x0_f64_x:
**	mov	z1\.d, x0
**	ret
*/
TEST_UNIFORM_ZS (dup_x0_f64_x, svfloat64_t, double,
		z1 = svdup_n_f64_x (p0, x0),
		z1 = svdup_f64_x (p0, x0))

/*
** dup_d0_f64_x:
**	mov	z1\.d, d0
**	ret
*/
TEST_UNIFORM_ZS (dup_d0_f64_x, svfloat64_t, double,
		z1 = svdup_n_f64_x (p0, d0),
		z1 = svdup_f64_x (p0, d0))

