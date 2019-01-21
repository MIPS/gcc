/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** set4_u64_z0_0:
**	mov	z1\.d, z17\.d
**	mov	z2\.d, z18\.d
**	mov	z3\.d, z19\.d
**	mov	z0\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z0_0, svuint64x4_t, svuint64_t,
	  z0 = svset4_u64 (z16, 0, z4),
	  z0 = svset4 (z16, 0, z4))

/*
** set4_u64_z0_1:
**	mov	z0\.d, z16\.d
**	mov	z2\.d, z18\.d
**	mov	z3\.d, z19\.d
**	mov	z1\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z0_1, svuint64x4_t, svuint64_t,
	  z0 = svset4_u64 (z16, 1, z4),
	  z0 = svset4 (z16, 1, z4))

/*
** set4_u64_z0_2:
**	mov	z0\.d, z16\.d
**	mov	z1\.d, z17\.d
**	mov	z3\.d, z19\.d
**	mov	z2\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z0_2, svuint64x4_t, svuint64_t,
	  z0 = svset4_u64 (z16, 2, z4),
	  z0 = svset4 (z16, 2, z4))

/*
** set4_u64_z0_3:
**	mov	z0\.d, z16\.d
**	mov	z1\.d, z17\.d
**	mov	z2\.d, z18\.d
**	mov	z3\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z0_3, svuint64x4_t, svuint64_t,
	  z0 = svset4_u64 (z16, 3, z4),
	  z0 = svset4 (z16, 3, z4))

/*
** set4_u64_z16_0:
**	mov	z16\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z16_0, svuint64x4_t, svuint64_t,
	  z16 = svset4_u64 (z16, 0, z4),
	  z16 = svset4 (z16, 0, z4))

/*
** set4_u64_z16_1:
**	mov	z17\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z16_1, svuint64x4_t, svuint64_t,
	  z16 = svset4_u64 (z16, 1, z4),
	  z16 = svset4 (z16, 1, z4))

/*
** set4_u64_z16_2:
**	mov	z18\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z16_2, svuint64x4_t, svuint64_t,
	  z16 = svset4_u64 (z16, 2, z4),
	  z16 = svset4 (z16, 2, z4))

/*
** set4_u64_z16_3:
**	mov	z19\.d, z4\.d
**	ret
*/
TEST_SET (set4_u64_z16_3, svuint64x4_t, svuint64_t,
	  z16 = svset4_u64 (z16, 3, z4),
	  z16 = svset4 (z16, 3, z4))
