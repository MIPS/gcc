/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vdivss\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vdivss\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 2 } } */
/* { dg-final { scan-assembler-times "vdivss\[ \\t\]+\[^\n\]*\{rn-sae\}\[^\n\]*%xmm\[0-9\]" 1 } } */
/* { dg-final { scan-assembler-times "vdivss\[ \\t\]+\[^\n\]*\{rd-sae\}\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vdivss\[ \\t\]+\[^\n\]*\{rz-sae\}\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m128 x1, x2;
volatile __mmask8 m;

void extern
avx512f_test (void)
{
  x1 = _mm_mask_div_ss (x1, m, x1, x2);
  x1 = _mm_maskz_div_ss (m, x1, x2);
  x1 = _mm_div_round_ss (x1, x2, _MM_FROUND_TO_NEAREST_INT);
  x1 = _mm_mask_div_round_ss (x1, m, x1, x2, _MM_FROUND_TO_NEG_INF);
  x1 = _mm_maskz_div_round_ss (m, x1, x2, _MM_FROUND_TO_ZERO);
}
