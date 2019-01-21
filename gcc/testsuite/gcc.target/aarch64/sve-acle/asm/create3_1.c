/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** create3_s8:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z3\.d
**	ret
*/
TEST_CREATE (create3_s8, svint8x3_t, svint8_t,
	     z16 = svcreate3_s8 (z2, z0, z3),
	     z16 = svcreate3 (z2, z0, z3))

/*
** create3_u8:
**	mov	z16\.d, z0\.d
**	mov	z17\.d, z2\.d
**	mov	z18\.d, z1\.d
**	ret
*/
TEST_CREATE (create3_u8, svuint8x3_t, svuint8_t,
	     z16 = svcreate3_u8 (z0, z2, z1),
	     z16 = svcreate3 (z0, z2, z1))

/*
** create3_s16:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z1\.d
**	ret
*/
TEST_CREATE (create3_s16, svint16x3_t, svint16_t,
	     z16 = svcreate3_s16 (z2, z0, z1),
	     z16 = svcreate3 (z2, z0, z1))

/*
** create3_u16:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z1\.d
**	mov	z18\.d, z0\.d
**	ret
*/
TEST_CREATE (create3_u16, svuint16x3_t, svuint16_t,
	     z16 = svcreate3_u16 (z2, z1, z0),
	     z16 = svcreate3 (z2, z1, z0))

/*
** create3_f16:
**	mov	z16\.d, z0\.d
**	mov	z17\.d, z1\.d
**	mov	z18\.d, z2\.d
**	ret
*/
TEST_CREATE (create3_f16, svfloat16x3_t, svfloat16_t,
	     z16 = svcreate3_f16 (z0, z1, z2),
	     z16 = svcreate3 (z0, z1, z2))

/*
** create3_s32:
**	mov	z16\.d, z2\.d
**	mov	z17\.d, z3\.d
**	mov	z18\.d, z0\.d
**	ret
*/
TEST_CREATE (create3_s32, svint32x3_t, svint32_t,
	     z16 = svcreate3_s32 (z2, z3, z0),
	     z16 = svcreate3 (z2, z3, z0))

/*
** create3_u32:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z1\.d
**	mov	z18\.d, z2\.d
**	ret
*/
TEST_CREATE (create3_u32, svuint32x3_t, svuint32_t,
	     z16 = svcreate3_u32 (z3, z1, z2),
	     z16 = svcreate3 (z3, z1, z2))

/*
** create3_f32:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z2\.d
**	ret
*/
TEST_CREATE (create3_f32, svfloat32x3_t, svfloat32_t,
	     z16 = svcreate3_f32 (z3, z0, z2),
	     z16 = svcreate3 (z3, z0, z2))

/*
** create3_s64:
**	mov	z16\.d, z1\.d
**	mov	z17\.d, z3\.d
**	mov	z18\.d, z2\.d
**	ret
*/
TEST_CREATE (create3_s64, svint64x3_t, svint64_t,
	     z16 = svcreate3_s64 (z1, z3, z2),
	     z16 = svcreate3 (z1, z3, z2))

/*
** create3_u64:
**	mov	z16\.d, z3\.d
**	mov	z17\.d, z2\.d
**	mov	z18\.d, z0\.d
**	ret
*/
TEST_CREATE (create3_u64, svuint64x3_t, svuint64_t,
	     z16 = svcreate3_u64 (z3, z2, z0),
	     z16 = svcreate3 (z3, z2, z0))

/*
** create3_f64:
**	mov	z16\.d, z1\.d
**	mov	z17\.d, z0\.d
**	mov	z18\.d, z3\.d
**	ret
*/
TEST_CREATE (create3_f64, svfloat64x3_t, svfloat64_t,
	     z16 = svcreate3_f64 (z1, z0, z3),
	     z16 = svcreate3 (z1, z0, z3))
