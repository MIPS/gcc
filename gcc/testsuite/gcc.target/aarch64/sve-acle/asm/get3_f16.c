/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** get3_f16_z0_0:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_GET (get3_f16_z0_0, svfloat16x3_t, svfloat16_t,
	  z0 = svget3_f16 (z16, 0),
	  z0 = svget3 (z16, 0))

/*
** get3_f16_z0_1:
**	mov	z0\.d, z17\.d
**	ret
*/
TEST_GET (get3_f16_z0_1, svfloat16x3_t, svfloat16_t,
	  z0 = svget3_f16 (z16, 1),
	  z0 = svget3 (z16, 1))

/*
** get3_f16_z0_2:
**	mov	z0\.d, z18\.d
**	ret
*/
TEST_GET (get3_f16_z0_2, svfloat16x3_t, svfloat16_t,
	  z0 = svget3_f16 (z16, 2),
	  z0 = svget3 (z16, 2))

/*
** get3_f16_z16_0:
**	ret
*/
TEST_GET (get3_f16_z16_0, svfloat16x3_t, svfloat16_t,
	  z16_res = svget3_f16 (z16, 0),
	  z16_res = svget3 (z16, 0))

/*
** get3_f16_z16_1:
**	mov	z16\.d, z17\.d
**	ret
*/
TEST_GET (get3_f16_z16_1, svfloat16x3_t, svfloat16_t,
	  z16_res = svget3_f16 (z16, 1),
	  z16_res = svget3 (z16, 1))

/*
** get3_f16_z16_2:
**	mov	z16\.d, z18\.d
**	ret
*/
TEST_GET (get3_f16_z16_2, svfloat16x3_t, svfloat16_t,
	  z16_res = svget3_f16 (z16, 2),
	  z16_res = svget3 (z16, 2))

/*
** get3_f16_z17_0:
**	mov	z17\.d, z16\.d
**	ret
*/
TEST_GET (get3_f16_z17_0, svfloat16x3_t, svfloat16_t,
	  z17_res = svget3_f16 (z16, 0),
	  z17_res = svget3 (z16, 0))

/*
** get3_f16_z17_1:
**	ret
*/
TEST_GET (get3_f16_z17_1, svfloat16x3_t, svfloat16_t,
	  z17_res = svget3_f16 (z16, 1),
	  z17_res = svget3 (z16, 1))

/*
** get3_f16_z17_2:
**	mov	z17\.d, z18\.d
**	ret
*/
TEST_GET (get3_f16_z17_2, svfloat16x3_t, svfloat16_t,
	  z17_res = svget3_f16 (z16, 2),
	  z17_res = svget3 (z16, 2))

/*
** get3_f16_z18_0:
**	mov	z18\.d, z16\.d
**	ret
*/
TEST_GET (get3_f16_z18_0, svfloat16x3_t, svfloat16_t,
	  z18_res = svget3_f16 (z16, 0),
	  z18_res = svget3 (z16, 0))

/*
** get3_f16_z18_1:
**	mov	z18\.d, z17\.d
**	ret
*/
TEST_GET (get3_f16_z18_1, svfloat16x3_t, svfloat16_t,
	  z18_res = svget3_f16 (z16, 1),
	  z18_res = svget3 (z16, 1))

/*
** get3_f16_z18_2:
**	ret
*/
TEST_GET (get3_f16_z18_2, svfloat16x3_t, svfloat16_t,
	  z18_res = svget3_f16 (z16, 2),
	  z18_res = svget3 (z16, 2))
