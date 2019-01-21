/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** expa_f32_tied1:
**	fexpa	z6\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (expa_f32_tied1, svfloat32_t, svuint32_t,
	     z6_res = svexpa_f32 (z6),
	     z6_res = svexpa (z6))

/*
** expa_f32_untied:
**	fexpa	z0\.s, z6\.s
**	ret
*/
TEST_DUAL_Z (expa_f32_untied, svfloat32_t, svuint32_t,
	     z0 = svexpa_f32 (z6),
	     z0 = svexpa (z6))
