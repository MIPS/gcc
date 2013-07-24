/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vminss\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vminss\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 2 } } */
/* { dg-final { scan-assembler-times "vminss\[ \\t\]+\[^\n\]*\{sae\}\[^\n\]*%xmm\[0-9\]\[^\{\]" 3 } } */
/* { dg-final { scan-assembler-times "vminss\[ \\t\]+\[^\n\]*\{sae\}\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vminss\[ \\t\]+\[^\n\]*\{sae\}\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m128 x1, x2;
volatile __mmask8 m;

void extern
avx512f_test (void)
{
  x1 = _mm_mask_min_ss (x1, m, x1, x2);
  x1 = _mm_maskz_min_ss (m, x1, x2);
  x1 = _mm_min_round_ss (x1, x2, _MM_FROUND_NO_EXC);
  x1 = _mm_mask_min_round_ss (x1, m, x1, x2, _MM_FROUND_NO_EXC);
  x1 = _mm_maskz_min_round_ss (m, x1, x2, _MM_FROUND_NO_EXC);
}
