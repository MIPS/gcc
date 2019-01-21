/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tbl_f32_tied1:
**	tbl	z0\.s, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tbl_f32_tied1, svfloat32_t, svuint32_t,
	     z0 = svtbl_f32 (z0, z6),
	     z0 = svtbl (z0, z6))

/*
** tbl_f32_tied2:
**	tbl	z6\.s, z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tbl_f32_tied2, svfloat32_t, svuint32_t,
	     z6_res = svtbl_f32 (z0, z6),
	     z6_res = svtbl (z0, z6))

/*
** tbl_f32_untied:
**	tbl	z0\.s, z1\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (tbl_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svtbl_f32 (z1, z6),
	     z0 = svtbl (z1, z6))
