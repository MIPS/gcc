/* { dg-options "-march=32r6" } */

unsigned int
foo (unsigned int x)
{
  return __builtin_bswap32 (x);
}

/* { dg-final { scan-assembler "byterevw" } } */
