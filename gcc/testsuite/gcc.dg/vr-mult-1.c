/* Check that the MIPS mulhi and muls patterns are used.

   Note: the architecture selected by dg-options might be overridden by
   --target_board.  Use a preprocessor test to check that we have the
   expected selection.  */

/* { dg-do compile { target mips*-*-* } } */
/* { dg-options "-march=vr5400 -O2" } */
#if _MIPS_ARCH_VR5400

/* mulhi and mulhiu allow a simpler constant divide sequence.
   gcc should use them in both 32-bit and 64-bit code.  */
int f1 (int x) { return x / 3; }
unsigned int f2 (unsigned int x) { return x / 3; }

/* In 64-bit code, combine should match these multiplications against
   the mulhi and mulhiu patterns.  It doesn't in 32-bit code
   because there's no explicit truncate:DI involved.  */
#if __mips64
int f3 (int x, int y)
{
  return ((long long) x * y) >> 32;
}
unsigned int f4 (unsigned int x, unsigned int y)
{
  return ((unsigned long long) x * y) >> 32;
}
#else
const char *f3 = "mulhi";
const char *f4 = "mulhiu";
#endif

/* Check that muls is used.  Should work in both 32-bit and
   64-bit code.  */
int f5 (int x, int y) { return -(x * y); }
#else
const char *foo = "mulhi//mulhiu//mulhi//mulhiu//muls";
#endif
/* { dg-final { scan-assembler "mulhi.*mulhiu.*mulhi.*mulhiu.*muls" } } */
