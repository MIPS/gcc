/* { dg-final { check-function-bodies "**" "" "-DCHECK_ASM" } } */

#include "test_sve_acle.h"

/*
** cmpge_u32_tied:
** (
**	cmphs	p0\.s, p0/z, z0\.s, z1\.s
** |
**	cmpls	p0\.s, p0/z, z1\.s, z0\.s
** )
**	ret
*/
TEST_COMPARE_Z (cmpge_u32_tied, svuint32_t,
		p0 = svcmpge_u32 (p0, z0, z1),
		p0 = svcmpge (p0, z0, z1))

/*
** cmpge_u32_untied:
** (
**	cmphs	p0\.s, p1/z, z0\.s, z1\.s
** |
**	cmpls	p0\.s, p1/z, z1\.s, z0\.s
** )
**	ret
*/
TEST_COMPARE_Z (cmpge_u32_untied, svuint32_t,
		p0 = svcmpge_u32 (p1, z0, z1),
		p0 = svcmpge (p1, z0, z1))

/*
** cmpge_x0_u32:
**	mov	(z[0-9]+\.d), x0
**	cmphs	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpge_x0_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, x0),
		 p0 = svcmpge (p1, z0, x0))

/*
** cmpge_d4_u32:
**	mov	(z[0-9]+\.d), d4
**	cmphs	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpge_d4_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, d4),
		 p0 = svcmpge (p1, z0, d4))

/*
** cmpge_0_u32:
**	cmphs	p0\.s, p1/z, z0\.s, #0
**	ret
*/
TEST_COMPARE_ZS (cmpge_0_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, 0),
		 p0 = svcmpge (p1, z0, 0))

/*
** cmpge_1_u32:
**	cmphs	p0\.s, p1/z, z0\.s, #1
**	ret
*/
TEST_COMPARE_ZS (cmpge_1_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, 1),
		 p0 = svcmpge (p1, z0, 1))

/*
** cmpge_15_u32:
**	cmphs	p0\.s, p1/z, z0\.s, #15
**	ret
*/
TEST_COMPARE_ZS (cmpge_15_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, 15),
		 p0 = svcmpge (p1, z0, 15))

/*
** cmpge_16_u32:
**	cmphs	p0\.s, p1/z, z0\.s, #16
**	ret
*/
TEST_COMPARE_ZS (cmpge_16_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, 16),
		 p0 = svcmpge (p1, z0, 16))

/*
** cmpge_127_u32:
**	cmphs	p0\.s, p1/z, z0\.s, #127
**	ret
*/
TEST_COMPARE_ZS (cmpge_127_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, 127),
		 p0 = svcmpge (p1, z0, 127))

/*
** cmpge_128_u32:
**	mov	(z[0-9]+\.d), #128
**	cmphs	p0\.s, p1/z, z0\.s, \1
**	ret
*/
TEST_COMPARE_ZS (cmpge_128_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, 128),
		 p0 = svcmpge (p1, z0, 128))

/*
** cmpge_m1_u32:
**	mov	(z[0-9]+)\.b, #-1
**	cmphs	p0\.s, p1/z, z0\.s, \1\.d
**	ret
*/
TEST_COMPARE_ZS (cmpge_m1_u32, svuint32_t, uint64_t,
		 p0 = svcmpge_n_u32 (p1, z0, -1),
		 p0 = svcmpge (p1, z0, -1))
