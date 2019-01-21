/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cvt_f32_f16_m_tied1:
**	fcvt	z0\.s, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f32_f16_m_tied1, svfloat32_t, svfloat16_t,
	     z0 = svcvt_f32_f16_m (z0, p0, z6),
	     z0 = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fcvt	z6\.s, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_f32_f16_m_tied2, svfloat32_t, svfloat16_t,
	     z6_res = svcvt_f32_f16_m (z0, p0, z6),
	     z6_res = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_f16_m_untied:
**	movprfx	z0, z1
**	fcvt	z0\.s, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f32_f16_m_untied, svfloat32_t, svfloat16_t,
	     z0 = svcvt_f32_f16_m (z1, p0, z6),
	     z0 = svcvt_f32_m (z1, p0, z6))

/*
** cvt_f32_f64_m_tied1:
**	fcvt	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_f64_m_tied1, svfloat32_t, svfloat64_t,
	     z0 = svcvt_f32_f64_m (z0, p0, z6),
	     z0 = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_f64_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	fcvt	z6\.s, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f32_f64_m_tied2, svfloat32_t, svfloat64_t,
	     z6_res = svcvt_f32_f64_m (z0, p0, z6),
	     z6_res = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_f64_m_untied:
**	movprfx	z0, z1
**	fcvt	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_f64_m_untied, svfloat32_t, svfloat64_t,
	     z0 = svcvt_f32_f64_m (z1, p0, z6),
	     z0 = svcvt_f32_m (z1, p0, z6))

/*
** cvt_f32_s32_m_tied1:
**	scvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_s32_m_tied1, svfloat32_t, svint32_t,
	     z0 = svcvt_f32_s32_m (z0, p0, z6),
	     z0 = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_s32_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	scvtf	z6\.s, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_s32_m_tied2, svfloat32_t, svint32_t,
	     z6_res = svcvt_f32_s32_m (z0, p0, z6),
	     z6_res = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_s32_m_untied:
**	movprfx	z0, z1
**	scvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_s32_m_untied, svfloat32_t, svint32_t,
	     z0 = svcvt_f32_s32_m (z1, p0, z6),
	     z0 = svcvt_f32_m (z1, p0, z6))

/*
** cvt_f32_s64_m_tied1:
**	scvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_s64_m_tied1, svfloat32_t, svint64_t,
	     z0 = svcvt_f32_s64_m (z0, p0, z6),
	     z0 = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_s64_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	scvtf	z6\.s, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f32_s64_m_tied2, svfloat32_t, svint64_t,
	     z6_res = svcvt_f32_s64_m (z0, p0, z6),
	     z6_res = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_s64_m_untied:
**	movprfx	z0, z1
**	scvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_s64_m_untied, svfloat32_t, svint64_t,
	     z0 = svcvt_f32_s64_m (z1, p0, z6),
	     z0 = svcvt_f32_m (z1, p0, z6))

/*
** cvt_f32_u32_m_tied1:
**	ucvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_u32_m_tied1, svfloat32_t, svuint32_t,
	     z0 = svcvt_f32_u32_m (z0, p0, z6),
	     z0 = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_u32_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	ucvtf	z6\.s, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_u32_m_tied2, svfloat32_t, svuint32_t,
	     z6_res = svcvt_f32_u32_m (z0, p0, z6),
	     z6_res = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_u32_m_untied:
**	movprfx	z0, z1
**	ucvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_u32_m_untied, svfloat32_t, svuint32_t,
	     z0 = svcvt_f32_u32_m (z1, p0, z6),
	     z0 = svcvt_f32_m (z1, p0, z6))

/*
** cvt_f32_u64_m_tied1:
**	ucvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_u64_m_tied1, svfloat32_t, svuint64_t,
	     z0 = svcvt_f32_u64_m (z0, p0, z6),
	     z0 = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_u64_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	ucvtf	z6\.s, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f32_u64_m_tied2, svfloat32_t, svuint64_t,
	     z6_res = svcvt_f32_u64_m (z0, p0, z6),
	     z6_res = svcvt_f32_m (z0, p0, z6))

/*
** cvt_f32_u64_m_untied:
**	movprfx	z0, z1
**	ucvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_u64_m_untied, svfloat32_t, svuint64_t,
	     z0 = svcvt_f32_u64_m (z1, p0, z6),
	     z0 = svcvt_f32_m (z1, p0, z6))

/*
** cvt_f32_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.s, p0/z, \1\.s
**	fcvt	z6\.s, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_f32_f16_z_tied1, svfloat32_t, svfloat16_t,
	     z6_res = svcvt_f32_f16_z (p0, z6),
	     z6_res = svcvt_f32_z (p0, z6))

/*
** cvt_f32_f16_z_untied:
**	movprfx	z0\.s, p0/z, z6\.s
**	fcvt	z0\.s, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f32_f16_z_untied, svfloat32_t, svfloat16_t,
	     z0 = svcvt_f32_f16_z (p0, z6),
	     z0 = svcvt_f32_z (p0, z6))

/*
** cvt_f32_f64_z_tied1:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.d, p0/z, \1
**	fcvt	z6\.s, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f32_f64_z_tied1, svfloat32_t, svfloat64_t,
	     z6_res = svcvt_f32_f64_z (p0, z6),
	     z6_res = svcvt_f32_z (p0, z6))

/*
** cvt_f32_f64_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	fcvt	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_f64_z_untied, svfloat32_t, svfloat64_t,
	     z0 = svcvt_f32_f64_z (p0, z6),
	     z0 = svcvt_f32_z (p0, z6))

/*
** cvt_f32_s32_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.s, p0/z, \1\.s
**	scvtf	z6\.s, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_s32_z_tied1, svfloat32_t, svint32_t,
	     z6_res = svcvt_f32_s32_z (p0, z6),
	     z6_res = svcvt_f32_z (p0, z6))

/*
** cvt_f32_s32_z_untied:
**	movprfx	z0\.s, p0/z, z6\.s
**	scvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_s32_z_untied, svfloat32_t, svint32_t,
	     z0 = svcvt_f32_s32_z (p0, z6),
	     z0 = svcvt_f32_z (p0, z6))

/*
** cvt_f32_s64_z_tied1:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.d, p0/z, \1
**	scvtf	z6\.s, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f32_s64_z_tied1, svfloat32_t, svint64_t,
	     z6_res = svcvt_f32_s64_z (p0, z6),
	     z6_res = svcvt_f32_z (p0, z6))

/*
** cvt_f32_s64_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	scvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_s64_z_untied, svfloat32_t, svint64_t,
	     z0 = svcvt_f32_s64_z (p0, z6),
	     z0 = svcvt_f32_z (p0, z6))

/*
** cvt_f32_u32_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.s, p0/z, \1\.s
**	ucvtf	z6\.s, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_u32_z_tied1, svfloat32_t, svuint32_t,
	     z6_res = svcvt_f32_u32_z (p0, z6),
	     z6_res = svcvt_f32_z (p0, z6))

/*
** cvt_f32_u32_z_untied:
**	movprfx	z0\.s, p0/z, z6\.s
**	ucvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_u32_z_untied, svfloat32_t, svuint32_t,
	     z0 = svcvt_f32_u32_z (p0, z6),
	     z0 = svcvt_f32_z (p0, z6))

/*
** cvt_f32_u64_z_tied1:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.d, p0/z, \1
**	ucvtf	z6\.s, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f32_u64_z_tied1, svfloat32_t, svuint64_t,
	     z6_res = svcvt_f32_u64_z (p0, z6),
	     z6_res = svcvt_f32_z (p0, z6))

/*
** cvt_f32_u64_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	ucvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_u64_z_untied, svfloat32_t, svuint64_t,
	     z0 = svcvt_f32_u64_z (p0, z6),
	     z0 = svcvt_f32_z (p0, z6))

/*
** cvt_f32_f16_x_tied1:
**	fcvt	z6\.s, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f32_f16_x_tied1, svfloat32_t, svfloat16_t,
	     z6_res = svcvt_f32_f16_x (p0, z6),
	     z6_res = svcvt_f32_x (p0, z6))

/*
** cvt_f32_f16_x_untied:
**	fcvt	z0\.s, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f32_f16_x_untied, svfloat32_t, svfloat16_t,
	     z0 = svcvt_f32_f16_x (p0, z6),
	     z0 = svcvt_f32_x (p0, z6))

/*
** cvt_f32_f64_x_tied1:
**	fcvt	z6\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_f64_x_tied1, svfloat32_t, svfloat64_t,
	     z6_res = svcvt_f32_f64_x (p0, z6),
	     z6_res = svcvt_f32_x (p0, z6))

/*
** cvt_f32_f64_x_untied:
**	fcvt	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_f64_x_untied, svfloat32_t, svfloat64_t,
	     z0 = svcvt_f32_f64_x (p0, z6),
	     z0 = svcvt_f32_x (p0, z6))

/*
** cvt_f32_s32_x_tied1:
**	scvtf	z6\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_s32_x_tied1, svfloat32_t, svint32_t,
	     z6_res = svcvt_f32_s32_x (p0, z6),
	     z6_res = svcvt_f32_x (p0, z6))

/*
** cvt_f32_s32_x_untied:
**	scvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_s32_x_untied, svfloat32_t, svint32_t,
	     z0 = svcvt_f32_s32_x (p0, z6),
	     z0 = svcvt_f32_x (p0, z6))

/*
** cvt_f32_s64_x_tied1:
**	scvtf	z6\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_s64_x_tied1, svfloat32_t, svint64_t,
	     z6_res = svcvt_f32_s64_x (p0, z6),
	     z6_res = svcvt_f32_x (p0, z6))

/*
** cvt_f32_s64_x_untied:
**	scvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_s64_x_untied, svfloat32_t, svint64_t,
	     z0 = svcvt_f32_s64_x (p0, z6),
	     z0 = svcvt_f32_x (p0, z6))

/*
** cvt_f32_u32_x_tied1:
**	ucvtf	z6\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_u32_x_tied1, svfloat32_t, svuint32_t,
	     z6_res = svcvt_f32_u32_x (p0, z6),
	     z6_res = svcvt_f32_x (p0, z6))

/*
** cvt_f32_u32_x_untied:
**	ucvtf	z0\.s, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f32_u32_x_untied, svfloat32_t, svuint32_t,
	     z0 = svcvt_f32_u32_x (p0, z6),
	     z0 = svcvt_f32_x (p0, z6))

/*
** cvt_f32_u64_x_tied1:
**	ucvtf	z6\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_u64_x_tied1, svfloat32_t, svuint64_t,
	     z6_res = svcvt_f32_u64_x (p0, z6),
	     z6_res = svcvt_f32_x (p0, z6))

/*
** cvt_f32_u64_x_untied:
**	ucvtf	z0\.s, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f32_u64_x_untied, svfloat32_t, svuint64_t,
	     z0 = svcvt_f32_u64_x (p0, z6),
	     z0 = svcvt_f32_x (p0, z6))

/*
** ptrue_cvt_f32_f16_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	fcvt	z6\.s, \1/m, z6\.h
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_f16_x_tied1, svfloat32_t, svfloat16_t,
	     z6_res = svcvt_f32_f16_x (svptrue_b32 (), z6),
	     z6_res = svcvt_f32_x (svptrue_b32 (), z6))

/*
** ptrue_cvt_f32_f16_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	fcvt	z0\.s, \1/m, z6\.h
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_f16_x_untied, svfloat32_t, svfloat16_t,
	     z0 = svcvt_f32_f16_x (svptrue_b32 (), z6),
	     z0 = svcvt_f32_x (svptrue_b32 (), z6))

/*
** ptrue_cvt_f32_f64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fcvt	z6\.s, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_f64_x_tied1, svfloat32_t, svfloat64_t,
	     z6_res = svcvt_f32_f64_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f32_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f32_f64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	fcvt	z0\.s, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_f64_x_untied, svfloat32_t, svfloat64_t,
	     z0 = svcvt_f32_f64_x (svptrue_b64 (), z6),
	     z0 = svcvt_f32_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f32_s32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	scvtf	z6\.s, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_s32_x_tied1, svfloat32_t, svint32_t,
	     z6_res = svcvt_f32_s32_x (svptrue_b32 (), z6),
	     z6_res = svcvt_f32_x (svptrue_b32 (), z6))

/*
** ptrue_cvt_f32_s32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	scvtf	z0\.s, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_s32_x_untied, svfloat32_t, svint32_t,
	     z0 = svcvt_f32_s32_x (svptrue_b32 (), z6),
	     z0 = svcvt_f32_x (svptrue_b32 (), z6))

/*
** ptrue_cvt_f32_s64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	scvtf	z6\.s, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_s64_x_tied1, svfloat32_t, svint64_t,
	     z6_res = svcvt_f32_s64_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f32_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f32_s64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	scvtf	z0\.s, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_s64_x_untied, svfloat32_t, svint64_t,
	     z0 = svcvt_f32_s64_x (svptrue_b64 (), z6),
	     z0 = svcvt_f32_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f32_u32_x_tied1:
**	ptrue	(p[0-7])\.s[^\n]*
**	ucvtf	z6\.s, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_u32_x_tied1, svfloat32_t, svuint32_t,
	     z6_res = svcvt_f32_u32_x (svptrue_b32 (), z6),
	     z6_res = svcvt_f32_x (svptrue_b32 (), z6))

/*
** ptrue_cvt_f32_u32_x_untied:
**	ptrue	(p[0-7])\.s[^\n]*
**	ucvtf	z0\.s, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_u32_x_untied, svfloat32_t, svuint32_t,
	     z0 = svcvt_f32_u32_x (svptrue_b32 (), z6),
	     z0 = svcvt_f32_x (svptrue_b32 (), z6))

/*
** ptrue_cvt_f32_u64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	ucvtf	z6\.s, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_u64_x_tied1, svfloat32_t, svuint64_t,
	     z6_res = svcvt_f32_u64_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f32_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f32_u64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	ucvtf	z0\.s, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f32_u64_x_untied, svfloat32_t, svuint64_t,
	     z0 = svcvt_f32_u64_x (svptrue_b64 (), z6),
	     z0 = svcvt_f32_x (svptrue_b64 (), z6))
