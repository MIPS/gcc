/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

int __attribute__ ((noinline, noclone))
foo (short *ptr1, short *ptr2)
{
  int res = 0;
  for (int i = 0; ptr1[i]; ++i)
    res += ptr2[i] * i;
  return res;
}

/* { dg-final { scan-assembler {\twrffr\t.*\tldff1h\t.*\tldff1h\t.*\trdffr\t} } } */
/* { dg-final { scan-assembler {\tincw\tz[0-9]} } } */
/* { dg-final { scan-assembler-times {\tmul\tz[0-9]} 2 } } */
