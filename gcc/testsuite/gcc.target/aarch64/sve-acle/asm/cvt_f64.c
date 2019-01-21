/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cvt_f64_f16_m_tied1:
**	fcvt	z0\.d, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_m_tied1, svfloat64_t, svfloat16_t,
	     z0 = svcvt_f64_f16_m (z0, p0, z6),
	     z0 = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_f16_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fcvt	z6\.d, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_m_tied2, svfloat64_t, svfloat16_t,
	     z6_res = svcvt_f64_f16_m (z0, p0, z6),
	     z6_res = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_f16_m_untied:
**	movprfx	z0, z1
**	fcvt	z0\.d, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_m_untied, svfloat64_t, svfloat16_t,
	     z0 = svcvt_f64_f16_m (z1, p0, z6),
	     z0 = svcvt_f64_m (z1, p0, z6))

/*
** cvt_f64_f32_m_tied1:
**	fcvt	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_m_tied1, svfloat64_t, svfloat32_t,
	     z0 = svcvt_f64_f32_m (z0, p0, z6),
	     z0 = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_f32_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	fcvt	z6\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_m_tied2, svfloat64_t, svfloat32_t,
	     z6_res = svcvt_f64_f32_m (z0, p0, z6),
	     z6_res = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_f32_m_untied:
**	movprfx	z0, z1
**	fcvt	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_m_untied, svfloat64_t, svfloat32_t,
	     z0 = svcvt_f64_f32_m (z1, p0, z6),
	     z0 = svcvt_f64_m (z1, p0, z6))

/*
** cvt_f64_s32_m_tied1:
**	scvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_m_tied1, svfloat64_t, svint32_t,
	     z0 = svcvt_f64_s32_m (z0, p0, z6),
	     z0 = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_s32_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	scvtf	z6\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_m_tied2, svfloat64_t, svint32_t,
	     z6_res = svcvt_f64_s32_m (z0, p0, z6),
	     z6_res = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_s32_m_untied:
**	movprfx	z0, z1
**	scvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_m_untied, svfloat64_t, svint32_t,
	     z0 = svcvt_f64_s32_m (z1, p0, z6),
	     z0 = svcvt_f64_m (z1, p0, z6))

/*
** cvt_f64_s64_m_tied1:
**	scvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_m_tied1, svfloat64_t, svint64_t,
	     z0 = svcvt_f64_s64_m (z0, p0, z6),
	     z0 = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_s64_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	scvtf	z6\.d, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_m_tied2, svfloat64_t, svint64_t,
	     z6_res = svcvt_f64_s64_m (z0, p0, z6),
	     z6_res = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_s64_m_untied:
**	movprfx	z0, z1
**	scvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_m_untied, svfloat64_t, svint64_t,
	     z0 = svcvt_f64_s64_m (z1, p0, z6),
	     z0 = svcvt_f64_m (z1, p0, z6))

/*
** cvt_f64_u32_m_tied1:
**	ucvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_m_tied1, svfloat64_t, svuint32_t,
	     z0 = svcvt_f64_u32_m (z0, p0, z6),
	     z0 = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_u32_m_tied2:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6, z0
**	ucvtf	z6\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_m_tied2, svfloat64_t, svuint32_t,
	     z6_res = svcvt_f64_u32_m (z0, p0, z6),
	     z6_res = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_u32_m_untied:
**	movprfx	z0, z1
**	ucvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_m_untied, svfloat64_t, svuint32_t,
	     z0 = svcvt_f64_u32_m (z1, p0, z6),
	     z0 = svcvt_f64_m (z1, p0, z6))

/*
** cvt_f64_u64_m_tied1:
**	ucvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_m_tied1, svfloat64_t, svuint64_t,
	     z0 = svcvt_f64_u64_m (z0, p0, z6),
	     z0 = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_u64_m_tied2:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6, z0
**	ucvtf	z6\.d, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_m_tied2, svfloat64_t, svuint64_t,
	     z6_res = svcvt_f64_u64_m (z0, p0, z6),
	     z6_res = svcvt_f64_m (z0, p0, z6))

/*
** cvt_f64_u64_m_untied:
**	movprfx	z0, z1
**	ucvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_m_untied, svfloat64_t, svuint64_t,
	     z0 = svcvt_f64_u64_m (z1, p0, z6),
	     z0 = svcvt_f64_m (z1, p0, z6))

/*
** cvt_f64_f16_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.d, p0/z, \1\.d
**	fcvt	z6\.d, p0/m, \1\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_z_tied1, svfloat64_t, svfloat16_t,
	     z6_res = svcvt_f64_f16_z (p0, z6),
	     z6_res = svcvt_f64_z (p0, z6))

/*
** cvt_f64_f16_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	fcvt	z0\.d, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_z_untied, svfloat64_t, svfloat16_t,
	     z0 = svcvt_f64_f16_z (p0, z6),
	     z0 = svcvt_f64_z (p0, z6))

/*
** cvt_f64_f32_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.d, p0/z, \1\.d
**	fcvt	z6\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_z_tied1, svfloat64_t, svfloat32_t,
	     z6_res = svcvt_f64_f32_z (p0, z6),
	     z6_res = svcvt_f64_z (p0, z6))

/*
** cvt_f64_f32_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	fcvt	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_z_untied, svfloat64_t, svfloat32_t,
	     z0 = svcvt_f64_f32_z (p0, z6),
	     z0 = svcvt_f64_z (p0, z6))

/*
** cvt_f64_s32_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.d, p0/z, \1\.d
**	scvtf	z6\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_z_tied1, svfloat64_t, svint32_t,
	     z6_res = svcvt_f64_s32_z (p0, z6),
	     z6_res = svcvt_f64_z (p0, z6))

/*
** cvt_f64_s32_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	scvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_z_untied, svfloat64_t, svint32_t,
	     z0 = svcvt_f64_s32_z (p0, z6),
	     z0 = svcvt_f64_z (p0, z6))

/*
** cvt_f64_s64_z_tied1:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.d, p0/z, \1
**	scvtf	z6\.d, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_z_tied1, svfloat64_t, svint64_t,
	     z6_res = svcvt_f64_s64_z (p0, z6),
	     z6_res = svcvt_f64_z (p0, z6))

/*
** cvt_f64_s64_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	scvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_z_untied, svfloat64_t, svint64_t,
	     z0 = svcvt_f64_s64_z (p0, z6),
	     z0 = svcvt_f64_z (p0, z6))

/*
** cvt_f64_u32_z_tied1:
**	mov	(z[0-9]+)\.d, z6\.d
**	movprfx	z6\.d, p0/z, \1\.d
**	ucvtf	z6\.d, p0/m, \1\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_z_tied1, svfloat64_t, svuint32_t,
	     z6_res = svcvt_f64_u32_z (p0, z6),
	     z6_res = svcvt_f64_z (p0, z6))

/*
** cvt_f64_u32_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	ucvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_z_untied, svfloat64_t, svuint32_t,
	     z0 = svcvt_f64_u32_z (p0, z6),
	     z0 = svcvt_f64_z (p0, z6))

/*
** cvt_f64_u64_z_tied1:
**	mov	(z[0-9]+\.d), z6\.d
**	movprfx	z6\.d, p0/z, \1
**	ucvtf	z6\.d, p0/m, \1
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_z_tied1, svfloat64_t, svuint64_t,
	     z6_res = svcvt_f64_u64_z (p0, z6),
	     z6_res = svcvt_f64_z (p0, z6))

/*
** cvt_f64_u64_z_untied:
**	movprfx	z0\.d, p0/z, z6\.d
**	ucvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_z_untied, svfloat64_t, svuint64_t,
	     z0 = svcvt_f64_u64_z (p0, z6),
	     z0 = svcvt_f64_z (p0, z6))

/*
** cvt_f64_f16_x_tied1:
**	fcvt	z6\.d, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_x_tied1, svfloat64_t, svfloat16_t,
	     z6_res = svcvt_f64_f16_x (p0, z6),
	     z6_res = svcvt_f64_x (p0, z6))

/*
** cvt_f64_f16_x_untied:
**	fcvt	z0\.d, p0/m, z6\.h
**	ret
*/
TEST_DUAL_Z (cvt_f64_f16_x_untied, svfloat64_t, svfloat16_t,
	     z0 = svcvt_f64_f16_x (p0, z6),
	     z0 = svcvt_f64_x (p0, z6))

/*
** cvt_f64_f32_x_tied1:
**	fcvt	z6\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_x_tied1, svfloat64_t, svfloat32_t,
	     z6_res = svcvt_f64_f32_x (p0, z6),
	     z6_res = svcvt_f64_x (p0, z6))

/*
** cvt_f64_f32_x_untied:
**	fcvt	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_f32_x_untied, svfloat64_t, svfloat32_t,
	     z0 = svcvt_f64_f32_x (p0, z6),
	     z0 = svcvt_f64_x (p0, z6))

/*
** cvt_f64_s32_x_tied1:
**	scvtf	z6\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_x_tied1, svfloat64_t, svint32_t,
	     z6_res = svcvt_f64_s32_x (p0, z6),
	     z6_res = svcvt_f64_x (p0, z6))

/*
** cvt_f64_s32_x_untied:
**	scvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_s32_x_untied, svfloat64_t, svint32_t,
	     z0 = svcvt_f64_s32_x (p0, z6),
	     z0 = svcvt_f64_x (p0, z6))

/*
** cvt_f64_s64_x_tied1:
**	scvtf	z6\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_x_tied1, svfloat64_t, svint64_t,
	     z6_res = svcvt_f64_s64_x (p0, z6),
	     z6_res = svcvt_f64_x (p0, z6))

/*
** cvt_f64_s64_x_untied:
**	scvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_s64_x_untied, svfloat64_t, svint64_t,
	     z0 = svcvt_f64_s64_x (p0, z6),
	     z0 = svcvt_f64_x (p0, z6))

/*
** cvt_f64_u32_x_tied1:
**	ucvtf	z6\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_x_tied1, svfloat64_t, svuint32_t,
	     z6_res = svcvt_f64_u32_x (p0, z6),
	     z6_res = svcvt_f64_x (p0, z6))

/*
** cvt_f64_u32_x_untied:
**	ucvtf	z0\.d, p0/m, z6\.s
**	ret
*/
TEST_DUAL_Z (cvt_f64_u32_x_untied, svfloat64_t, svuint32_t,
	     z0 = svcvt_f64_u32_x (p0, z6),
	     z0 = svcvt_f64_x (p0, z6))

/*
** cvt_f64_u64_x_tied1:
**	ucvtf	z6\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_x_tied1, svfloat64_t, svuint64_t,
	     z6_res = svcvt_f64_u64_x (p0, z6),
	     z6_res = svcvt_f64_x (p0, z6))

/*
** cvt_f64_u64_x_untied:
**	ucvtf	z0\.d, p0/m, z6\.d
**	ret
*/
TEST_DUAL_Z (cvt_f64_u64_x_untied, svfloat64_t, svuint64_t,
	     z0 = svcvt_f64_u64_x (p0, z6),
	     z0 = svcvt_f64_x (p0, z6))

/*
** ptrue_cvt_f64_f16_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fcvt	z6\.d, \1/m, z6\.h
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_f16_x_tied1, svfloat64_t, svfloat16_t,
	     z6_res = svcvt_f64_f16_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_f16_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	fcvt	z0\.d, \1/m, z6\.h
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_f16_x_untied, svfloat64_t, svfloat16_t,
	     z0 = svcvt_f64_f16_x (svptrue_b64 (), z6),
	     z0 = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_f32_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	fcvt	z6\.d, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_f32_x_tied1, svfloat64_t, svfloat32_t,
	     z6_res = svcvt_f64_f32_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_f32_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	fcvt	z0\.d, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_f32_x_untied, svfloat64_t, svfloat32_t,
	     z0 = svcvt_f64_f32_x (svptrue_b64 (), z6),
	     z0 = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_s32_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	scvtf	z6\.d, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_s32_x_tied1, svfloat64_t, svint32_t,
	     z6_res = svcvt_f64_s32_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_s32_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	scvtf	z0\.d, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_s32_x_untied, svfloat64_t, svint32_t,
	     z0 = svcvt_f64_s32_x (svptrue_b64 (), z6),
	     z0 = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_s64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	scvtf	z6\.d, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_s64_x_tied1, svfloat64_t, svint64_t,
	     z6_res = svcvt_f64_s64_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_s64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	scvtf	z0\.d, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_s64_x_untied, svfloat64_t, svint64_t,
	     z0 = svcvt_f64_s64_x (svptrue_b64 (), z6),
	     z0 = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_u32_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	ucvtf	z6\.d, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_u32_x_tied1, svfloat64_t, svuint32_t,
	     z6_res = svcvt_f64_u32_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_u32_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	ucvtf	z0\.d, \1/m, z6\.s
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_u32_x_untied, svfloat64_t, svuint32_t,
	     z0 = svcvt_f64_u32_x (svptrue_b64 (), z6),
	     z0 = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_u64_x_tied1:
**	ptrue	(p[0-7])\.d[^\n]*
**	ucvtf	z6\.d, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_u64_x_tied1, svfloat64_t, svuint64_t,
	     z6_res = svcvt_f64_u64_x (svptrue_b64 (), z6),
	     z6_res = svcvt_f64_x (svptrue_b64 (), z6))

/*
** ptrue_cvt_f64_u64_x_untied:
**	ptrue	(p[0-7])\.d[^\n]*
**	ucvtf	z0\.d, \1/m, z6\.d
**	ret
*/
TEST_DUAL_Z (ptrue_cvt_f64_u64_x_untied, svfloat64_t, svuint64_t,
	     z0 = svcvt_f64_u64_x (svptrue_b64 (), z6),
	     z0 = svcvt_f64_x (svptrue_b64 (), z6))
