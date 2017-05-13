/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

int __attribute__ ((weak))
f (int x)
{
  for (int i = 0;; i++)
    if (x & (1 << i))
      return i;
}

/* { dg-final { scan-assembler {\tcmpne\tp[0-9]+\.s,} } } */
/* { dg-final { scan-assembler {\tbrka\t} } } */
/* { dg-final { scan-assembler {\tlastb\t} } } */
