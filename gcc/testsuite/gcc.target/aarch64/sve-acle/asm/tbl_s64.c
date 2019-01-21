/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** tbl_s64_tied1:
**	tbl	z0\.d, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (tbl_s64_tied1, svint64_t, svuint64_t,
	     z0 = svtbl_s64 (z0, z6),
	     z0 = svtbl (z0, z6))

/*
** tbl_s64_tied2:
**	tbl	z6\.d, z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (tbl_s64_tied2, svint64_t, svuint64_t,
	     z6_res = svtbl_s64 (z0, z6),
	     z6_res = svtbl (z0, z6))

/*
** tbl_s64_untied:
**	tbl	z0\.d, z1\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (tbl_s64_untied, svint64_t, svuint64_t,
	     z0 = svtbl_s64 (z1, z6),
	     z0 = svtbl (z1, z6))
