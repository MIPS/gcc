/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256" } */

int mask[] = { 1, 5, 9, 11, 15, 19, 100, 101, 102, 0xf8 };

int __attribute__ ((noinline, noclone))
foo (void)
{
  int res = 0;
  int i = -1;
  do
    {
      i += 1;
      res += i;
    }
  while (mask[i] != 0xf8);
  return res;
}


/* { dg-final { scan-assembler {\tld1w\t.*\tbrka\t.*\tadd\tz[0-9]\.s, p[0-7]/m,} } } */
/* { dg-final { scan-assembler-not {\twhilelo\t} } } */
