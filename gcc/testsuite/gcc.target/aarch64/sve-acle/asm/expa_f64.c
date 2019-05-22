/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** expa_f64_tied1:
**	fexpa	z16\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (expa_f64_tied1, svfloat64_t, svuint64_t,
	     z16_res = svexpa_f64 (z16),
	     z16_res = svexpa (z16))

/*
** expa_f64_untied:
**	fexpa	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (expa_f64_untied, svfloat64_t, svuint64_t,
	     z0 = svexpa_f64 (z16),
	     z0 = svexpa (z16))
