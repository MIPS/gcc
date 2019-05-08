/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tbl_f32_tied1:
**	tbl	z0\.s, z0\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tbl_f32_tied1, svfloat32_t, svuint32_t,
	     z0 = svtbl_f32 (z0, z16),
	     z0 = svtbl (z0, z16))

/*
** tbl_f32_tied2:
**	tbl	z16\.s, z0\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tbl_f32_tied2, svfloat32_t, svuint32_t,
	     z16_res = svtbl_f32 (z0, z16),
	     z16_res = svtbl (z0, z16))

/*
** tbl_f32_untied:
**	tbl	z0\.s, z1\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (tbl_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svtbl_f32 (z1, z16),
	     z0 = svtbl (z1, z16))
