/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpkhi_s16_tied1:
**	sunpkhi	z6\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpkhi_s16_tied1, svint16_t, svint8_t,
	     z6_res = svunpkhi_s16 (z6),
	     z6_res = svunpkhi (z6))

/*
** unpkhi_s16_untied:
**	sunpkhi	z0\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpkhi_s16_untied, svint16_t, svint8_t,
	     z0 = svunpkhi_s16 (z6),
	     z0 = svunpkhi (z6))
