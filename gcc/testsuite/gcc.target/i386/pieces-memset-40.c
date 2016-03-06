/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mno-avx512f -mavx2 -mtune=sandybridge" } */

extern char *dst;

void
foo (int x)
{
  __builtin_memset (dst, x, 66);
}

/* { dg-final { scan-assembler-times "vpbroadcastb\[ \\t\]+\[^\n\]*%xmm" 1 } } */
/* { dg-final { scan-assembler-times "vmovups\[ \\t\]+\[^\n\]*%xmm" 4 } } */
