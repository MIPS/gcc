/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vrcp14sd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\{\]" 3 } } */
/* { dg-final { scan-assembler-times "vrcp14sd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vrcp14sd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m128d x1, x2;
volatile __mmask8 m;

void extern
avx512f_test (void)
{
  x1 = _mm_rcp14_sd (x1, x2);
  x1 = _mm_mask_rcp14_sd (x1, m, x1, x2);
  x1 = _mm_maskz_rcp14_sd (m, x1, x2);
}
