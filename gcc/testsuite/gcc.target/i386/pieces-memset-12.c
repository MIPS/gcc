/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mavx512f -mtune=generic" } */

extern char *dst;

void
foo (void)
{
  __builtin_memset (dst, 3, 66);
}

/* { dg-final { scan-assembler-times "vmovups\[ \\t\]+\[^\n\]*%xmm" 4 } } */
