/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** asr_wide_s32_m_tied1:
**	asr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_m_tied1, svint32_t,
	     z0 = svasr_wide_s32_m (p0, z0, z16),
	     z0 = svasr_wide_m (p0, z0, z16))

/*
** asr_wide_s32_m_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16, z0
**	asr	z16\.s, p0/m, z16\.s, \1
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_m_tied2, svint32_t,
	     z16_res = svasr_wide_s32_m (p0, z0, z16),
	     z16_res = svasr_wide_m (p0, z0, z16))

/*
** asr_wide_s32_m_untied:
**	movprfx	z0, z1
**	asr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_m_untied, svint32_t,
	     z0 = svasr_wide_s32_m (p0, z1, z16),
	     z0 = svasr_wide_m (p0, z1, z16))

/*
** asr_wide_s32_z_tied1:
**	movprfx	z0\.s, p0/z, z0\.s
**	asr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_z_tied1, svint32_t,
	     z0 = svasr_wide_s32_z (p0, z0, z16),
	     z0 = svasr_wide_z (p0, z0, z16))

/*
** asr_wide_s32_z_tied2:
**	mov	(z[0-9]+\.d), z16\.d
**	movprfx	z16\.s, p0/z, z0\.s
**	asr	z16\.s, p0/m, z16\.s, \1
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_z_tied2, svint32_t,
	     z16_res = svasr_wide_s32_z (p0, z0, z16),
	     z16_res = svasr_wide_z (p0, z0, z16))

/*
** asr_wide_s32_z_untied:
**	movprfx	z0\.s, p0/z, z1\.s
**	asr	z0\.s, p0/m, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_z_untied, svint32_t,
	     z0 = svasr_wide_s32_z (p0, z1, z16),
	     z0 = svasr_wide_z (p0, z1, z16))

/*
** asr_wide_s32_x_tied1:
**	asr	z0\.s, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_x_tied1, svint32_t,
	     z0 = svasr_wide_s32_x (p0, z0, z16),
	     z0 = svasr_wide_x (p0, z0, z16))

/*
** asr_wide_s32_x_tied2:
**	asr	z16\.s, z0\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_x_tied2, svint32_t,
	     z16_res = svasr_wide_s32_x (p0, z0, z16),
	     z16_res = svasr_wide_x (p0, z0, z16))

/*
** asr_wide_s32_x_untied:
**	asr	z0\.s, z1\.s, z16\.d
**	ret
*/
TEST_WIDE_Z (asr_wide_s32_x_untied, svint32_t,
	     z0 = svasr_wide_s32_x (p0, z1, z16),
	     z0 = svasr_wide_x (p0, z1, z16))
