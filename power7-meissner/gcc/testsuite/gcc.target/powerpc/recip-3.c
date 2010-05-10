/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-O2 -mrecip -mrecip-precision -ffast-math -mcpu=power7" } */
/* { dg-final { scan-assembler-times "xsrsqrtedp" 1 } } */
/* { dg-final { scan-assembler-times "xsmsub.dp" 1 } } */
/* { dg-final { scan-assembler-times "xsmuldp" 4 } } */
/* { dg-final { scan-assembler-times "xsnmsub.dp" 2 } } */

double rsqrt_d (double a)
{
  return 1.0 / __builtin_sqrt (a);
}
