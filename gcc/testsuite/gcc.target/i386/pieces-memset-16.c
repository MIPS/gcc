/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mno-avx -msse2 -mtune=generic" } */

extern char *dst;

void
foo (void)
{
  __builtin_memset (dst, 3, 17);
}

/* { dg-final { scan-assembler-times "movups\[ \\t\]+\[^\n\]*%xmm" 1 } } */
