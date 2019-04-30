/* { dg-do compile } */
/* { dg-options "-O" } */
/* { dg-final { check-function-bodies "**" "" } } */

#include <arm_sve.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
** test_s8:
**	ptrue	(p[0-7])\.b, all
**	ld1b	(z[0-9]+\.b), \1/z, \[x0\]
**	add	\2, \2, #1
**	st1b	\2, \1, \[x1\]
**	ret
*/
void
test_s8 (int8_t *x, int8_t *y)
{
  int8_t tmp1[32], tmp2[32];

  svbool_t pg = svptrue_b8 ();
  svst1 (pg, tmp1, svld1 (pg, x));
  svst1 (pg, tmp2, svadd_x (pg, svld1 (pg, tmp1), 1));
  svst1 (pg, y, svld1 (pg, tmp2));
}

#ifdef __cplusplus
}
#endif
