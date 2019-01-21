/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** set3_f16_z0_0:
**	mov	z1\.d, z17\.d
**	mov	z2\.d, z18\.d
**	mov	z0\.d, z4\.d
**	ret
*/
TEST_SET (set3_f16_z0_0, svfloat16x3_t, svfloat16_t,
	  z0 = svset3_f16 (z16, 0, z4),
	  z0 = svset3 (z16, 0, z4))

/*
** set3_f16_z0_1:
**	mov	z0\.d, z16\.d
**	mov	z2\.d, z18\.d
**	mov	z1\.d, z4\.d
**	ret
*/
TEST_SET (set3_f16_z0_1, svfloat16x3_t, svfloat16_t,
	  z0 = svset3_f16 (z16, 1, z4),
	  z0 = svset3 (z16, 1, z4))

/*
** set3_f16_z0_2:
**	mov	z0\.d, z16\.d
**	mov	z1\.d, z17\.d
**	mov	z2\.d, z4\.d
**	ret
*/
TEST_SET (set3_f16_z0_2, svfloat16x3_t, svfloat16_t,
	  z0 = svset3_f16 (z16, 2, z4),
	  z0 = svset3 (z16, 2, z4))

/*
** set3_f16_z16_0:
**	mov	z16\.d, z4\.d
**	ret
*/
TEST_SET (set3_f16_z16_0, svfloat16x3_t, svfloat16_t,
	  z16 = svset3_f16 (z16, 0, z4),
	  z16 = svset3 (z16, 0, z4))

/*
** set3_f16_z16_1:
**	mov	z17\.d, z4\.d
**	ret
*/
TEST_SET (set3_f16_z16_1, svfloat16x3_t, svfloat16_t,
	  z16 = svset3_f16 (z16, 1, z4),
	  z16 = svset3 (z16, 1, z4))

/*
** set3_f16_z16_2:
**	mov	z18\.d, z4\.d
**	ret
*/
TEST_SET (set3_f16_z16_2, svfloat16x3_t, svfloat16_t,
	  z16 = svset3_f16 (z16, 2, z4),
	  z16 = svset3 (z16, 2, z4))
