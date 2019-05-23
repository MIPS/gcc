/* { dg-do compile } */
/* { dg-options "-O2" } */

#include <arm_sve.h>

void
test1 (svbool_t prev, int *last, svbool_t *ptr)
{
  svbool_t res = svpnext_b16 (svptrue_b16 (), prev);
  *last = svptest_last (svptrue_b16 (), res);
  *ptr = res;
}

int
test2 (svbool_t prev)
{
  svbool_t res = svpnext_b16 (svptrue_b16 (), prev);
  return svptest_last (svptrue_b16 (), res);
}

void
test3 (svbool_t prev, int *last, svbool_t *ptr)
{
  svbool_t res = svpnext_b32 (svptrue_b32 (), prev);
  *last = svptest_last (svptrue_b32 (), res);
  *ptr = res;
}

int
test4 (svbool_t prev)
{
  svbool_t res = svpnext_b32 (svptrue_b32 (), prev);
  return svptest_last (svptrue_b32 (), res);
}

void
test5 (svbool_t prev, int *last, svbool_t *ptr)
{
  svbool_t res = svpnext_b64 (svptrue_b64 (), prev);
  *last = svptest_last (svptrue_b64 (), res);
  *ptr = res;
}

int
test6 (svbool_t prev)
{
  svbool_t res = svpnext_b64 (svptrue_b64 (), prev);
  return svptest_last (svptrue_b64 (), res);
}

/* { dg-final { scan-assembler-times {\tpnext\t} 6 } } */
/* { dg-final { scan-assembler-not {\tptest\t} } } */
