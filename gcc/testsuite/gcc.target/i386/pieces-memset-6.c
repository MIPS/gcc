/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mavx512f -mtune=intel" } */

extern char *dst;

void
foo (int x)
{
  __builtin_memset (dst, x, 33);
}

/* { dg-final { scan-assembler-times "vmovdqu\[ \\t\]+\[^\n\]*%xmm" 2 } } */
