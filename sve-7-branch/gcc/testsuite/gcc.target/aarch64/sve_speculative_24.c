/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

int __attribute__ ((noinline, noclone))
foo (int *mask)
{
  int res = 0;
  for (int i = 0; mask[i] != 0xf8; ++i)
    res += i;
  return res;
}

/* { dg-final { scan-assembler {\twrffr\t.*\tldff1w\t.*\trdffr\t} } } */
/* { dg-final { scan-assembler {\trdffr\t.*\tbrka\t.*\tadd\tz[0-9]\.s, p[0-7]/m,} } } */
