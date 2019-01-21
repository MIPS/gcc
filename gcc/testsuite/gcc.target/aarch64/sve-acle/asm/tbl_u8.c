/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tbl_u8_tied1:
**	tbl	z0\.b, z0\.b, z6\.b
**	ret
*/
TEST_DUAL_Z (tbl_u8_tied1, svuint8_t, svuint8_t,
	     z0 = svtbl_u8 (z0, z6),
	     z0 = svtbl (z0, z6))

/*
** tbl_u8_tied2:
**	tbl	z6\.b, z0\.b, z6\.b
**	ret
*/
TEST_DUAL_Z (tbl_u8_tied2, svuint8_t, svuint8_t,
	     z6_res = svtbl_u8 (z0, z6),
	     z6_res = svtbl (z0, z6))

/*
** tbl_u8_untied:
**	tbl	z0\.b, z1\.b, z6\.b
**	ret
*/
TEST_DUAL_Z (tbl_u8_untied, svuint8_t, svuint8_t,
	     z0 = svtbl_u8 (z1, z6),
	     z0 = svtbl (z1, z6))
