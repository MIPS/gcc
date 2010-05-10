/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-O2 -mrecip -mno-recip-precision -ffast-math -mcpu=power5" } */
/* { dg-final { scan-assembler-times "frsqrte" 1 } } */
/* { dg-final { scan-assembler-times "fmsubs" 1 } } */
/* { dg-final { scan-assembler-times "fmuls" 6 } } */
/* { dg-final { scan-assembler-times "fnmsubs" 3 } } */

float rsqrt_f (float a)
{
  return 1.0f / __builtin_sqrtf (a);
}
