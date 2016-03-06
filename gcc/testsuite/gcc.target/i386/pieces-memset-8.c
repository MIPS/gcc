/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mno-avx2 -mavx -mtune=generic" } */

extern char *dst;

void
foo (int x)
{
  __builtin_memset (dst, x, 17);
}

/* { dg-final { scan-assembler-times "vmovups\[ \\t\]+\[^\n\]*%xmm" 1 } } */
