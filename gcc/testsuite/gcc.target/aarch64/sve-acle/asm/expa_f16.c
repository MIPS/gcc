/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** expa_f16_tied1:
**	fexpa	z6\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (expa_f16_tied1, svfloat16_t, svuint16_t,
	     z6_res = svexpa_f16 (z6),
	     z6_res = svexpa (z6))

/*
** expa_f16_untied:
**	fexpa	z0\.h, z6\.h
**	ret
*/
TEST_DUAL_Z (expa_f16_untied, svfloat16_t, svuint16_t,
	     z0 = svexpa_f16 (z6),
	     z0 = svexpa (z6))
