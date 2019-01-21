/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** create2_s8:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z0\.d
**	ret
*/
TEST_CREATE (create2_s8, svint8x2_t, svint8_t,
	     z16 = svcreate2_s8 (z2, z0),
	     z16 = svcreate2 (z2, z0))

/*
** create2_u8:
**	mov	z16\.d, z0\.d
**	mov	z17\.d, z2\.d
**	ret
*/
TEST_CREATE (create2_u8, svuint8x2_t, svuint8_t,
	     z16 = svcreate2_u8 (z0, z2),
	     z16 = svcreate2 (z0, z2))

/*
** create2_s16:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z0\.d
**	ret
*/
TEST_CREATE (create2_s16, svint16x2_t, svint16_t,
	     z16 = svcreate2_s16 (z2, z0),
	     z16 = svcreate2 (z2, z0))

/*
** create2_u16:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z1\.d
**	ret
*/
TEST_CREATE (create2_u16, svuint16x2_t, svuint16_t,
	     z16 = svcreate2_u16 (z2, z1),
	     z16 = svcreate2 (z2, z1))

/*
** create2_f16:
**	mov	z16\.d, z0\.d
**	mov	z17\.d, z1\.d
**	ret
*/
TEST_CREATE (create2_f16, svfloat16x2_t, svfloat16_t,
	     z16 = svcreate2_f16 (z0, z1),
	     z16 = svcreate2 (z0, z1))

/*
** create2_s32:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z3\.d
**	ret
*/
TEST_CREATE (create2_s32, svint32x2_t, svint32_t,
	     z16 = svcreate2_s32 (z2, z3),
	     z16 = svcreate2 (z2, z3))

/*
** create2_u32:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z1\.d
**	ret
*/
TEST_CREATE (create2_u32, svuint32x2_t, svuint32_t,
	     z16 = svcreate2_u32 (z3, z1),
	     z16 = svcreate2 (z3, z1))

/*
** create2_f32:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z0\.d
**	ret
*/
TEST_CREATE (create2_f32, svfloat32x2_t, svfloat32_t,
	     z16 = svcreate2_f32 (z3, z0),
	     z16 = svcreate2 (z3, z0))

/*
** create2_s64:
**	mov	z16\.d, z1\.d
**	mov	z17\.d, z3\.d
**	ret
*/
TEST_CREATE (create2_s64, svint64x2_t, svint64_t,
	     z16 = svcreate2_s64 (z1, z3),
	     z16 = svcreate2 (z1, z3))

/*
** create2_u64:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z2\.d
**	ret
*/
TEST_CREATE (create2_u64, svuint64x2_t, svuint64_t,
	     z16 = svcreate2_u64 (z3, z2),
	     z16 = svcreate2 (z3, z2))

/*
** create2_f64:
**	mov	z16\.d, z1\.d
**	mov	z17\.d, z0\.d
**	ret
*/
TEST_CREATE (create2_f64, svfloat64x2_t, svfloat64_t,
	     z16 = svcreate2_f64 (z1, z0),
	     z16 = svcreate2 (z1, z0))
