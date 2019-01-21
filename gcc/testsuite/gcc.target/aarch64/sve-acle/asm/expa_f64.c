/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** expa_f64_tied1:
**	fexpa	z6\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (expa_f64_tied1, svfloat64_t, svuint64_t,
	     z6_res = svexpa_f64 (z6),
	     z6_res = svexpa (z6))

/*
** expa_f64_untied:
**	fexpa	z0\.d, z6\.d
**	ret
*/
TEST_DUAL_Z (expa_f64_untied, svfloat64_t, svuint64_t,
	     z0 = svexpa_f64 (z6),
	     z0 = svexpa (z6))
