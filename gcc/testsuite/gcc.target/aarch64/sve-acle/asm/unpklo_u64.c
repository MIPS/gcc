/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_u64_tied1:
**	uunpklo	z6\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpklo_u64_tied1, svuint64_t, svuint32_t,
	     z6_res = svunpklo_u64 (z6),
	     z6_res = svunpklo (z6))

/*
** unpklo_u64_untied:
**	uunpklo	z0\.d, z6\.s
**	ret
*/
TEST_DUAL_Z (unpklo_u64_untied, svuint64_t, svuint32_t,
	     z0 = svunpklo_u64 (z6),
	     z0 = svunpklo (z6))
