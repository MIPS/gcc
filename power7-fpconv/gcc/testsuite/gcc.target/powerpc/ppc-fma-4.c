/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_altivec_ok } */
/* { dg-options "-O3 -ftree-vectorize -mcpu=power6 -maltivec -ffast-math -mno-fused-madd" } */
/* { dg-final { scan-assembler-times "vmaddfp" 1 } } */
/* { dg-final { scan-assembler-times "fmadd " 1 } } */
/* { dg-final { scan-assembler-times "fmadds" 1 } } */

/* Only the functions calling the builtin should generate an appropriate
   (a * b) + c instruction.  */

double
builtin_fma (double b, double c, double d)
{
  return __builtin_fma (b, c, d);
}

float
builtin_fmaf (float b, float c, float d)
{
  return __builtin_fmaf (b, c, d);
}

double
normal_fma (double b, double c, double d)
{
  return (b * c) + d;
}

float
normal_fmaf (float b, float c, float d)
{
  return (b * c) + d;
}

#ifndef SIZE
#define SIZE 1024
#endif

float vfa[SIZE] __attribute__((__aligned__(32)));
float vfb[SIZE] __attribute__((__aligned__(32)));
float vfc[SIZE] __attribute__((__aligned__(32)));
float vfd[SIZE] __attribute__((__aligned__(32)));

void
vector_fmaf (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    vfa[i] = __builtin_fmaf (vfb[i], vfc[i], vfd[i]);
}

void
vnormal_fmaf (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    vfa[i] = (vfb[i] * vfc[i]) + vfd[i];
}
