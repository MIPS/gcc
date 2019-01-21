/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** dot_s64_tied1:
**	sdot	z0\.d, z6\.h, z7\.h
**	ret
*/
TEST_DUAL_Z (dot_s64_tied1, svint64_t, svint16_t,
	     z0 = svdot_s64 (z0, z6, z7),
	     z0 = svdot (z0, z6, z7))

/*
** dot_s64_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	sdot	z6\.d, \1\.h, z7\.h
**	ret
*/
TEST_DUAL_Z (dot_s64_tied2, svint64_t, svint16_t,
	     z6_res = svdot_s64 (z0, z6, z7),
	     z6_res = svdot (z0, z6, z7))

/*
** dot_s64_tied3:
**	mov	(z[0-9]+)\.d, z7\.d
**	movprfx	z7, z0
**	sdot	z7\.d, z6\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (dot_s64_tied3, svint64_t, svint16_t,
	     z7_res = svdot_s64 (z0, z6, z7),
	     z7_res = svdot (z0, z6, z7))

/*
** dot_s64_untied:
**	movprfx	z0, z1
**	sdot	z0\.d, z6\.h, z7\.h
**	ret
*/
TEST_DUAL_Z (dot_s64_untied, svint64_t, svint16_t,
	     z0 = svdot_s64 (z1, z6, z7),
	     z0 = svdot (z1, z6, z7))

/*
** dot_w0_s64_tied1:
**	mov	(z[0-9]+\.h), w0
**	sdot	z0\.d, z6\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_s64_tied1, svint64_t, svint16_t, int16_t,
	      z0 = svdot_n_s64 (z0, z6, x0),
	      z0 = svdot (z0, z6, x0))

/*
** dot_w0_s64_tied2:
**	mov	(z[0-9]+\.h), w0
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	sdot	z6\.d, \2\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_s64_tied2, svint64_t, svint16_t, int16_t,
	      z6_res = svdot_n_s64 (z0, z6, x0),
	      z6_res = svdot (z0, z6, x0))

/*
** dot_w0_s64_untied:
**	mov	(z[0-9]+\.h), w0
**	movprfx	z0, z1
**	sdot	z0\.d, z6\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_w0_s64_untied, svint64_t, svint16_t, int16_t,
	      z0 = svdot_n_s64 (z1, z6, x0),
	      z0 = svdot (z1, z6, x0))

/*
** dot_h4_s64_untied:
**	mov	(z[0-9]+\.h), h4
**	movprfx	z0, z1
**	sdot	z0\.d, z6\.h, \1
**	ret
*/
TEST_DUAL_ZS (dot_h4_s64_untied, svint64_t, svint16_t, int16_t,
	      z0 = svdot_n_s64 (z1, z6, d4),
	      z0 = svdot (z1, z6, d4))

/*
** dot_2_s64_untied:
**	mov	(z[0-9]+\.h), #2
**	movprfx	z0, z1
**	sdot	z0\.d, z6\.h, \1
**	ret
*/
TEST_DUAL_Z (dot_2_s64_untied, svint64_t, svint16_t,
	     z0 = svdot_n_s64 (z1, z6, 2),
	     z0 = svdot (z1, z6, 2))

/*
** dot_m1_s64:
**	mov	(z[0-9]+)\.b, #-1
**	sdot	z0\.d, z6\.h, \1\.h
**	ret
*/
TEST_DUAL_Z (dot_m1_s64, svint64_t, svint16_t,
	     z0 = svdot_n_s64 (z0, z6, -1),
	     z0 = svdot (z0, z6, -1))
