/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tbl_u32_tied1:
**	tbl	z0\.s, z0\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tbl_u32_tied1, svuint32_t, svuint32_t,
	     z0 = svtbl_u32 (z0, z16),
	     z0 = svtbl (z0, z16))

/*
** tbl_u32_tied2:
**	tbl	z16\.s, z0\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tbl_u32_tied2, svuint32_t, svuint32_t,
	     z16_res = svtbl_u32 (z0, z16),
	     z16_res = svtbl (z0, z16))

/*
** tbl_u32_untied:
**	tbl	z0\.s, z1\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tbl_u32_untied, svuint32_t, svuint32_t,
	     z0 = svtbl_u32 (z1, z16),
	     z0 = svtbl (z1, z16))
