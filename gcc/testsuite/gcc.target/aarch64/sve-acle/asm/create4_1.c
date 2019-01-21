/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** create4_s8:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z3\.d
**	mov	z19\.d, z1\.d
**	ret
*/
TEST_CREATE (create4_s8, svint8x4_t, svint8_t,
	     z16 = svcreate4_s8 (z2, z0, z3, z1),
	     z16 = svcreate4 (z2, z0, z3, z1))

/*
** create4_u8:
**	mov	z16\.d, z0\.d
**	mov	z17\.d, z2\.d
**	mov	z18\.d, z1\.d
**	mov	z19\.d, z3\.d
**	ret
*/
TEST_CREATE (create4_u8, svuint8x4_t, svuint8_t,
	     z16 = svcreate4_u8 (z0, z2, z1, z3),
	     z16 = svcreate4 (z0, z2, z1, z3))

/*
** create4_s16:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z1\.d
**	mov	z19\.d, z3\.d
**	ret
*/
TEST_CREATE (create4_s16, svint16x4_t, svint16_t,
	     z16 = svcreate4_s16 (z2, z0, z1, z3),
	     z16 = svcreate4 (z2, z0, z1, z3))

/*
** create4_u16:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z1\.d
**	mov	z18\.d, z0\.d
**	mov	z19\.d, z3\.d
**	ret
*/
TEST_CREATE (create4_u16, svuint16x4_t, svuint16_t,
	     z16 = svcreate4_u16 (z2, z1, z0, z3),
	     z16 = svcreate4 (z2, z1, z0, z3))

/*
** create4_f16:
**	mov	z16\.d, z0\.d
**	mov	z17\.d, z1\.d
**	mov	z18\.d, z2\.d
**	mov	z19\.d, z3\.d
**	ret
*/
TEST_CREATE (create4_f16, svfloat16x4_t, svfloat16_t,
	     z16 = svcreate4_f16 (z0, z1, z2, z3),
	     z16 = svcreate4 (z0, z1, z2, z3))

/*
** create4_s32:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z3\.d
**	mov	z18\.d, z0\.d
**	mov	z19\.d, z1\.d
**	ret
*/
TEST_CREATE (create4_s32, svint32x4_t, svint32_t,
	     z16 = svcreate4_s32 (z2, z3, z0, z1),
	     z16 = svcreate4 (z2, z3, z0, z1))

/*
** create4_u32:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z1\.d
**	mov	z18\.d, z2\.d
**	mov	z19\.d, z3\.d
**	ret
*/
TEST_CREATE (create4_u32, svuint32x4_t, svuint32_t,
	     z16 = svcreate4_u32 (z3, z1, z2, z3),
	     z16 = svcreate4 (z3, z1, z2, z3))

/*
** create4_f32:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z2\.d
**	mov	z19\.d, z0\.d
**	ret
*/
TEST_CREATE (create4_f32, svfloat32x4_t, svfloat32_t,
	     z16 = svcreate4_f32 (z3, z0, z2, z0),
	     z16 = svcreate4 (z3, z0, z2, z0))

/*
** create4_s64:
**	mov	z16\.d, z1\.d
**	mov	z17\.d, z3\.d
**	mov	z18\.d, z2\.d
**	mov	z19\.d, z2\.d
**	ret
*/
TEST_CREATE (create4_s64, svint64x4_t, svint64_t,
	     z16 = svcreate4_s64 (z1, z3, z2, z2),
	     z16 = svcreate4 (z1, z3, z2, z2))

/*
** create4_u64:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z2\.d
**	mov	z18\.d, z0\.d
**	mov	z19\.d, z1\.d
**	ret
*/
TEST_CREATE (create4_u64, svuint64x4_t, svuint64_t,
	     z16 = svcreate4_u64 (z3, z2, z0, z1),
	     z16 = svcreate4 (z3, z2, z0, z1))

/*
** create4_f64:
**	mov	z16\.d, z1\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z3\.d
**	mov	z19\.d, z2\.d
**	ret
*/
TEST_CREATE (create4_f64, svfloat64x4_t, svfloat64_t,
	     z16 = svcreate4_f64 (z1, z0, z3, z2),
	     z16 = svcreate4 (z1, z0, z3, z2))
