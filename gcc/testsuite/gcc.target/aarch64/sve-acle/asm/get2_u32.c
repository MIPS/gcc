/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** get2_u32_z0_0:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_GET (get2_u32_z0_0, svuint32x2_t, svuint32_t,
	  z0 = svget2_u32 (z16, 0),
	  z0 = svget2 (z16, 0))

/*
** get2_u32_z0_1:
**	mov	z0\.d, z17\.d
**	ret
*/
TEST_GET (get2_u32_z0_1, svuint32x2_t, svuint32_t,
	  z0 = svget2_u32 (z16, 1),
	  z0 = svget2 (z16, 1))

/*
** get2_u32_z16_0:
**	ret
*/
TEST_GET (get2_u32_z16_0, svuint32x2_t, svuint32_t,
	  z16_res = svget2_u32 (z16, 0),
	  z16_res = svget2 (z16, 0))

/*
** get2_u32_z16_1:
**	mov	z16\.d, z17\.d
**	ret
*/
TEST_GET (get2_u32_z16_1, svuint32x2_t, svuint32_t,
	  z16_res = svget2_u32 (z16, 1),
	  z16_res = svget2 (z16, 1))

/*
** get2_u32_z17_0:
**	mov	z17\.d, z16\.d
**	ret
*/
TEST_GET (get2_u32_z17_0, svuint32x2_t, svuint32_t,
	  z17_res = svget2_u32 (z16, 0),
	  z17_res = svget2 (z16, 0))

/*
** get2_u32_z17_1:
**	ret
*/
TEST_GET (get2_u32_z17_1, svuint32x2_t, svuint32_t,
	  z17_res = svget2_u32 (z16, 1),
	  z17_res = svget2 (z16, 1))
