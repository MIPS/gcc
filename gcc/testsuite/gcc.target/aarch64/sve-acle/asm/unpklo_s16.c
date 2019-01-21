/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** unpklo_s16_tied1:
**	sunpklo	z6\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpklo_s16_tied1, svint16_t, svint8_t,
	     z6_res = svunpklo_s16 (z6),
	     z6_res = svunpklo (z6))

/*
** unpklo_s16_untied:
**	sunpklo	z0\.h, z6\.b
**	ret
*/
TEST_DUAL_Z (unpklo_s16_untied, svint16_t, svint8_t,
	     z0 = svunpklo_s16 (z6),
	     z0 = svunpklo (z6))
