/* { dg-do run { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-options "-O2 -mcpu=power5 -std=c99 -fno-builtin-fma -fno-builtin-fmaf" } */

/* Test whether -fno-builtin-fma, -fno-builtin-fmaf properly turn off the
   macros math.h uses for FP_FAST_FMA{,F,L}.  */
#ifdef __FP_FAST_FMA
#error "__FP_FAST_FMA should not be defined"
#endif

#ifdef __FP_FAST_FMAF
#error "__FP_FAST_FMAF should not be defined"
#endif

int
main (void)
{
  return 0;
}
