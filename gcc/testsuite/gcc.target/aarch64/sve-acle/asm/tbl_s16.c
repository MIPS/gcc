/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tbl_s16_tied1:
**	tbl	z0\.h, z0\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (tbl_s16_tied1, svint16_t, svuint16_t,
	     z0 = svtbl_s16 (z0, z16),
	     z0 = svtbl (z0, z16))

/*
** tbl_s16_tied2:
**	tbl	z16\.h, z0\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (tbl_s16_tied2, svint16_t, svuint16_t,
	     z16_res = svtbl_s16 (z0, z16),
	     z16_res = svtbl (z0, z16))

/*
** tbl_s16_untied:
**	tbl	z0\.h, z1\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (tbl_s16_untied, svint16_t, svuint16_t,
	     z0 = svtbl_s16 (z1, z16),
	     z0 = svtbl (z1, z16))
