/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

float __attribute__ ((weak))
f (int x, float y)
{
  float f = 0.0f;
  for (int i = 0;; i++, f += y)
    if (x & (1 << i))
      return f;
}

/* { dg-final { scan-assembler {\tcmpne\tp[0-9]+\.s,} } } */
/* { dg-final { scan-assembler {\tbrka\t} } } */
/* { dg-final { scan-assembler {\tfadda\t} } } */
