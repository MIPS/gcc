/* { dg-do compile } */
/* { dg-options "-O2" } */

#include <arm_sve.h>

void foo (void);

void
test1 (int32_t x, svbool_t prev)
{
  svbool_t res = svpnext_b8 (svptrue_b8 (), prev);
  if (!svptest_last (svptrue_b8 (), res) && x == 0)
    foo ();
}

void
test2 (int32_t x, svbool_t prev)
{
  svbool_t res = svpnext_b16 (svptrue_b16 (), prev);
  if (!svptest_last (svptrue_b16 (), res) && x == 0)
    foo ();
}

void
test3 (int32_t x, svbool_t prev)
{
  svbool_t res = svpnext_b32 (svptrue_b32 (), prev);
  if (!svptest_last (svptrue_b32 (), res) && x == 0)
    foo ();
}

void
test4 (int32_t x, svbool_t prev)
{
  svbool_t res = svpnext_b64 (svptrue_b64 (), prev);
  if (!svptest_last (svptrue_b64 (), res) && x == 0)
    foo ();
}

/* { dg-final { scan-assembler-times {\tpnext\t} 4 } } */
/* { dg-final { scan-assembler-times {\tptrue\t} 4 } } */
/* { dg-final { scan-assembler-times {\tccmp\tw0, 0, 0, nlast\n} 4 } } */
/* { dg-final { scan-assembler-times {\tccmp\t} 4 } } */
/* { dg-final { scan-assembler-not {\tptest\t} } } */
/* { dg-final { scan-assembler-not {\tcmp\t} } } */
