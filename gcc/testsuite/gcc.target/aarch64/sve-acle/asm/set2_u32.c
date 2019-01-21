/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** set2_u32_z0_0:
**	mov	z1\.d, z17\.d
**	mov	z0\.d, z4\.d
**	ret
*/
TEST_SET (set2_u32_z0_0, svuint32x2_t, svuint32_t,
	  z0 = svset2_u32 (z16, 0, z4),
	  z0 = svset2 (z16, 0, z4))

/*
** set2_u32_z0_1:
**	mov	z0\.d, z16\.d
**	mov	z1\.d, z4\.d
**	ret
*/
TEST_SET (set2_u32_z0_1, svuint32x2_t, svuint32_t,
	  z0 = svset2_u32 (z16, 1, z4),
	  z0 = svset2 (z16, 1, z4))

/*
** set2_u32_z16_0:
**	mov	z16\.d, z4\.d
**	ret
*/
TEST_SET (set2_u32_z16_0, svuint32x2_t, svuint32_t,
	  z16 = svset2_u32 (z16, 0, z4),
	  z16 = svset2 (z16, 0, z4))

/*
** set2_u32_z16_1:
**	mov	z17\.d, z4\.d
**	ret
*/
TEST_SET (set2_u32_z16_1, svuint32x2_t, svuint32_t,
	  z16 = svset2_u32 (z16, 1, z4),
	  z16 = svset2 (z16, 1, z4))
