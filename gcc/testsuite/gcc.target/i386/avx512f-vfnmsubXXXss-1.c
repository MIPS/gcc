/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vfnmsub...ss\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 4 } } */
/* { dg-final { scan-assembler-times "vfnmsub231ss\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vfnmsub...ss\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 2 } } */
/* { dg-final { scan-assembler-times "vfnmsub...ss\[ \\t\]+\[^\n\]*\{rn-sae\}\[^\n\]*%xmm\[0-9\]\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfnmsub...ss\[ \\t\]+\[^\n\]*\{rd-sae\}\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfnmsub231ss\[ \\t\]+\[^\n\]*\{ru-sae\}\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vfnmsub...ss\[ \\t\]+\[^\n\]*\{rz-sae\}\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m128 a, b, c;
volatile __mmask8 m;

void extern
avx512f_test (void)
{
  a = _mm_mask_fnmsub_ss (a, m, b, c);
  c = _mm_mask3_fnmsub_ss (a, b, c, m);
  a = _mm_maskz_fnmsub_ss (m, a, b, c);
  a = _mm_fnmsub_round_ss (a, b, c, _MM_FROUND_TO_NEAREST_INT);
  a = _mm_mask_fnmsub_round_ss (a, m, b, c, _MM_FROUND_TO_NEG_INF);
  c = _mm_mask3_fnmsub_round_ss (a, b, c, m, _MM_FROUND_TO_POS_INF);
  a = _mm_maskz_fnmsub_round_ss (m, a, b, c, _MM_FROUND_TO_ZERO);
}
