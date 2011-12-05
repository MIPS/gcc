/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-O2 -ffast-math -mcpu=power5 -mno-altivec -mabi=altivec -fno-unroll-loops" } */
/* { dg-final { scan-assembler-times "vaddfp" 1 } } */
/* { dg-final { scan-assembler-times "xvaddsp" 1 } } */
/* { dg-final { scan-assembler-times "fadds" 1 } } */

#ifndef SIZE
#define SIZE 1024
#endif

#ifdef __ALTIVEC__
#error "__ALTIVEC__ should not be defined."
#endif

#ifdef __VSX__
#error "__VSX__ should not be defined."
#endif

#pragma GCC push_options
#pragma GCC target("altivec,no-vsx")

#ifndef __ALTIVEC__
#error "__ALTIVEC__ should be defined."
#endif

#ifdef __VSX__
#error "__VSX__ should not be defined."
#endif

void av_add (float *a, float *b, float *c)
{
  unsigned long i;
  unsigned long n = SIZE / 4;
  __vector float *av_a = (__vector float *)a;
  __vector float *av_b = (__vector float *)b;
  __vector float *av_c = (__vector float *)c;

  for (i = 0; i < n; i++)
    av_a[i] = __builtin_altivec_vaddfp (av_b[i], av_c[i]);
}

#pragma GCC target("vsx")

#ifndef __ALTIVEC__
#error "__ALTIVEC__ should be defined."
#endif

#ifndef __VSX__
#error "__VSX__ should be defined."
#endif

void vsx_add (float *a, float *b, float *c)
{
  unsigned long i;
  unsigned long n = SIZE / 4;
  __vector float *vsx_a = (__vector float *)a;
  __vector float *vsx_b = (__vector float *)b;
  __vector float *vsx_c = (__vector float *)c;

  for (i = 0; i < n; i++)
    vsx_a[i] = __builtin_vsx_xvaddsp (vsx_b[i], vsx_c[i]);
}

#pragma GCC pop_options
#pragma GCC target("no-vsx,no-altivec")

#ifdef __ALTIVEC__
#error "__ALTIVEC__ should not be defined."
#endif

#ifdef __VSX__
#error "__VSX__ should not be defined."
#endif

void norm_add (float *a, float *b, float *c)
{
  unsigned long i;

  for (i = 0; i < SIZE; i++)
    a[i] = b[i] + c[i];
}
