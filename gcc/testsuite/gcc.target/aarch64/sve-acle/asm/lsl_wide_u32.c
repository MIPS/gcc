/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** lsl_wide_u32_m_tied1:
**	lsl	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_m_tied1, svuint32_t,
	     z0 = svlsl_wide_u32_m (p0, z0, z16),
	     z0 = svlsl_wide_m (p0, z0, z16))

/*
** lsl_wide_u32_m_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_m_untied, svuint32_t,
	     z0 = svlsl_wide_u32_m (p0, z1, z16),
	     z0 = svlsl_wide_m (p0, z1, z16))

/* Bad RA choice: no preferred output sequence.  */
TEST_WIDE_Z (lsl_wide_u32_m_tied2, svuint32_t,
	     z16_res = svlsl_wide_u32_m (p0, z0, z16),
	     z16_res = svlsl_wide_m (p0, z0, z16))

/*
** lsl_wide_u32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	lsl	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_z_tied1, svuint32_t,
	     z0 = svlsl_wide_u32_z (p0, z0, z16),
	     z0 = svlsl_wide_z (p0, z0, z16))

/*
** lsl_wide_u32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	lsl	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_z_untied, svuint32_t,
	     z0 = svlsl_wide_u32_z (p0, z1, z16),
	     z0 = svlsl_wide_z (p0, z1, z16))

/* Bad RA choice: no preferred output sequence.  */
TEST_WIDE_Z (lsl_wide_u32_z_tied2, svuint32_t,
	     z16_res = svlsl_wide_u32_z (p0, z0, z16),
	     z16_res = svlsl_wide_z (p0, z0, z16))

/*
** lsl_wide_u32_x_tied1:
**	lsl	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_x_tied1, svuint32_t,
	     z0 = svlsl_wide_u32_x (p0, z0, z16),
	     z0 = svlsl_wide_x (p0, z0, z16))

/*
** lsl_wide_u32_x_untied:
**	movprfx	z0, z1
**	lsl	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (lsl_wide_u32_x_untied, svuint32_t,
	     z0 = svlsl_wide_u32_x (p0, z1, z16),
	     z0 = svlsl_wide_x (p0, z1, z16))

/* Bad RA choice: no preferred output sequence.  */
TEST_WIDE_Z (lsl_wide_u32_x_tied2, svuint32_t,
	     z16_res = svlsl_wide_u32_x (p0, z0, z16),
	     z16_res = svlsl_wide_x (p0, z0, z16))
