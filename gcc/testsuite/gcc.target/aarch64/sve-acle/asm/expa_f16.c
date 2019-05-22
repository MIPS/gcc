/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** expa_f16_tied1:
**	fexpa	z16\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (expa_f16_tied1, svfloat16_t, svuint16_t,
	     z16_res = svexpa_f16 (z16),
	     z16_res = svexpa (z16))

/*
** expa_f16_untied:
**	fexpa	z0\.h, z16\.h
**	ret
*/
TEST_DUAL_Z (expa_f16_untied, svfloat16_t, svuint16_t,
	     z0 = svexpa_f16 (z16),
	     z0 = svexpa (z16))
