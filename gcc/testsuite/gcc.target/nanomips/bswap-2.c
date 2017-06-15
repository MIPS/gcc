/* { dg-options "-march=32r6" } */

unsigned short
foo (unsigned short x)
{
  return __builtin_bswap16 (x);
}

/* { dg-final { scan-assembler "\twsbh\t" } } */
