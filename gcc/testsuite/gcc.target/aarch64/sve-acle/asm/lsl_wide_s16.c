/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_wide_s16_m_tied1:
**	lsl	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_m_tied1, svint16_t,
	     z0 = svlsl_wide_s16_m (p0, z0, z16),
	     z0 = svlsl_wide_m (p0, z0, z16))

/*
** lsl_wide_s16_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_m_untied, svint16_t,
	     z0 = svlsl_wide_s16_m (p0, z1, z16),
	     z0 = svlsl_wide_m (p0, z1, z16))

/* Bad RA choice: no preferred output sequence.  */
TEST_WIDE_Z (lsl_wide_s16_m_tied2, svint16_t,
	     z16_res = svlsl_wide_s16_m (p0, z0, z16),
	     z16_res = svlsl_wide_m (p0, z0, z16))

/*
** lsl_wide_s16_z_tied1:
**	movprfx	z0\.h, p0/z, z0\.h
**	lsl	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_z_tied1, svint16_t,
	     z0 = svlsl_wide_s16_z (p0, z0, z16),
	     z0 = svlsl_wide_z (p0, z0, z16))

/*
** lsl_wide_s16_z_untied:
**	movprfx	z0\.h, p0/z, z1\.h
**	lsl	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_z_untied, svint16_t,
	     z0 = svlsl_wide_s16_z (p0, z1, z16),
	     z0 = svlsl_wide_z (p0, z1, z16))

/* Bad RA choice: no preferred output sequence.  */
TEST_WIDE_Z (lsl_wide_s16_z_tied2, svint16_t,
	     z16_res = svlsl_wide_s16_z (p0, z0, z16),
	     z16_res = svlsl_wide_z (p0, z0, z16))

/*
** lsl_wide_s16_x_tied1:
**	lsl	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_x_tied1, svint16_t,
	     z0 = svlsl_wide_s16_x (p0, z0, z16),
	     z0 = svlsl_wide_x (p0, z0, z16))

/*
** lsl_wide_s16_x_untied:
**	movprfx	z0, z1
**	lsl	z0\.h, p0/m, z0\.h, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_s16_x_untied, svint16_t,
	     z0 = svlsl_wide_s16_x (p0, z1, z16),
	     z0 = svlsl_wide_x (p0, z1, z16))

/* Bad RA choice: no preferred output sequence.  */
TEST_WIDE_Z (lsl_wide_s16_x_tied2, svint16_t,
	     z16_res = svlsl_wide_s16_x (p0, z0, z16),
	     z16_res = svlsl_wide_x (p0, z0, z16))
