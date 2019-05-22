/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** expa_f32_tied1:
**	fexpa	z16\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (expa_f32_tied1, svfloat32_t, svuint32_t,
	     z16_res = svexpa_f32 (z16),
	     z16_res = svexpa (z16))

/*
** expa_f32_untied:
**	fexpa	z0\.s, z16\.s
**	ret
*/
TEST_DUAL_Z (expa_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svexpa_f32 (z16),
	     z0 = svexpa (z16))
