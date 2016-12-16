/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

int
foo (int *mask)
{
  int res = 0;
  for (int i = 0; mask[i] != 0xf8; ++i)
    res += i;
  return res;
}

/* { dg-final { scan-assembler {\twrffr\t.*\tldff1w\t.*\trdffr\t} } } */
