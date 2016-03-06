/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mno-avx -msse2 -mtune=generic" } */

extern char *dst;

void
foo (int x)
{
  __builtin_memset (dst, x, 33);
}

/* { dg-final { scan-assembler-times "movups\[ \\t\]+\[^\n\]*%xmm" 2 } } */
