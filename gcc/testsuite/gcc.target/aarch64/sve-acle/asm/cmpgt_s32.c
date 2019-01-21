/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpgt_s32_tied:
** (
**	cmpgt	p0\.s, p0/z, z0\.s, z1\.s
** |
**	cmplt	p0\.s, p0/z, z1\.s, z0\.s
** )
**	ret
*/
TEST_COMPARE_Z (cmpgt_s32_tied, svint32_t,
		p0 = svcmpgt_s32 (p0, z0, z1),
		p0 = svcmpgt (p0, z0, z1))

/*
** cmpgt_s32_untied:
** (
**	cmpgt	p0\.s, p1/z, z0\.s, z1\.s
** |
**	cmplt	p0\.s, p1/z, z1\.s, z0\.s
** )
**	ret
*/
TEST_COMPARE_Z (cmpgt_s32_untied, svint32_t,
		p0 = svcmpgt_s32 (p1, z0, z1),
		p0 = svcmpgt (p1, z0, z1))

/*
** cmpgt_x0_s32:
**	mov	(z[0-9]+\.d), x0
**	cmpgt	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_x0_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, x0),
		 p0 = svcmpgt (p1, z0, x0))

/*
** cmpgt_d4_s32:
**	mov	(z[0-9]+\.d), d4
**	cmpgt	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_d4_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, d4),
		 p0 = svcmpgt (p1, z0, d4))

/*
** cmpgt_0_s32:
**	cmpgt	p0\.s, p1/z, z0\.s, #0
**	ret
*/
TEST_COMPARE_ZS (cmpgt_0_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, 0),
		 p0 = svcmpgt (p1, z0, 0))

/*
** cmpgt_1_s32:
**	cmpgt	p0\.s, p1/z, z0\.s, #1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_1_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, 1),
		 p0 = svcmpgt (p1, z0, 1))

/*
** cmpgt_15_s32:
**	cmpgt	p0\.s, p1/z, z0\.s, #15
**	ret
*/
TEST_COMPARE_ZS (cmpgt_15_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, 15),
		 p0 = svcmpgt (p1, z0, 15))

/*
** cmpgt_16_s32:
**	mov	(z[0-9]+\.d), #16
**	cmpgt	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_16_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, 16),
		 p0 = svcmpgt (p1, z0, 16))

/*
** cmpgt_m1_s32:
**	cmpgt	p0\.s, p1/z, z0\.s, #-1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_m1_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, -1),
		 p0 = svcmpgt (p1, z0, -1))

/*
** cmpgt_m16_s32:
**	cmpgt	p0\.s, p1/z, z0\.s, #-16
**	ret
*/
TEST_COMPARE_ZS (cmpgt_m16_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, -16),
		 p0 = svcmpgt (p1, z0, -16))

/*
** cmpgt_m17_s32:
**	mov	(z[0-9]+\.d), #-17
**	cmpgt	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpgt_m17_s32, svint32_t, int64_t,
		 p0 = svcmpgt_n_s32 (p1, z0, -17),
		 p0 = svcmpgt (p1, z0, -17))
