/* { dg-do compile } */
/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** reinterpret_s32_f16_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_f16_tied1, svint32_t, svfloat16_t,
	     z16_res = svreinterpret_s32_f16 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_f16_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_f16_untied, svint32_t, svfloat16_t,
	     z0 = svreinterpret_s32_f16 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_f32_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_f32_tied1, svint32_t, svfloat32_t,
	     z16_res = svreinterpret_s32_f32 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_f32_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_f32_untied, svint32_t, svfloat32_t,
	     z0 = svreinterpret_s32_f32 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_f64_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_f64_tied1, svint32_t, svfloat64_t,
	     z16_res = svreinterpret_s32_f64 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_f64_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_f64_untied, svint32_t, svfloat64_t,
	     z0 = svreinterpret_s32_f64 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s16_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s16_tied1, svint32_t, svint16_t,
	     z16_res = svreinterpret_s32_s16 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s16_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s16_untied, svint32_t, svint16_t,
	     z0 = svreinterpret_s32_s16 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s32_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s32_tied1, svint32_t, svint32_t,
	     z16_res = svreinterpret_s32_s32 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s32_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s32_untied, svint32_t, svint32_t,
	     z0 = svreinterpret_s32_s32 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s64_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s64_tied1, svint32_t, svint64_t,
	     z16_res = svreinterpret_s32_s64 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s64_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s64_untied, svint32_t, svint64_t,
	     z0 = svreinterpret_s32_s64 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s8_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s8_tied1, svint32_t, svint8_t,
	     z16_res = svreinterpret_s32_s8 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_s8_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_s8_untied, svint32_t, svint8_t,
	     z0 = svreinterpret_s32_s8 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u16_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u16_tied1, svint32_t, svuint16_t,
	     z16_res = svreinterpret_s32_u16 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u16_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u16_untied, svint32_t, svuint16_t,
	     z0 = svreinterpret_s32_u16 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u32_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u32_tied1, svint32_t, svuint32_t,
	     z16_res = svreinterpret_s32_u32 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u32_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u32_untied, svint32_t, svuint32_t,
	     z0 = svreinterpret_s32_u32 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u64_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u64_tied1, svint32_t, svuint64_t,
	     z16_res = svreinterpret_s32_u64 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u64_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u64_untied, svint32_t, svuint64_t,
	     z0 = svreinterpret_s32_u64 (z16),
	     z0 = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u8_tied1:
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u8_tied1, svint32_t, svuint8_t,
	     z16_res = svreinterpret_s32_u8 (z16),
	     z16_res = svreinterpret_s32 (z16))

/*
** reinterpret_s32_u8_untied:
**	mov	z0\.d, z16\.d
**	ret
*/
TEST_DUAL_Z (reinterpret_s32_u8_untied, svint32_t, svuint8_t,
	     z0 = svreinterpret_s32_u8 (z16),
	     z0 = svreinterpret_s32 (z16))
