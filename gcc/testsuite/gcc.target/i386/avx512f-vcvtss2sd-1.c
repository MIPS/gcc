/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vcvtss2sd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 2 } } */
/* { dg-final { scan-assembler-times "vcvtss2sd\[ \\t\]+\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 2 } } */
/* { dg-final { scan-assembler-times "vcvtss2sd\[ \\t\]+\[^\n\]*\{sae\}\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\[^\{\]" 3 } } */
/* { dg-final { scan-assembler-times "vcvtss2sd\[ \\t\]+\[^\n\]*\{sae\}\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 1 } } */
/* { dg-final { scan-assembler-times "vcvtss2sd\[ \\t\]+\[^\n\]*\{sae\}\[^\n\]*%xmm\[0-9\]\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 1 } } */

#include <immintrin.h>

volatile __m128d s1, r;
volatile __m128 s2;
volatile __mmask8 m;

void extern
avx512f_test (void)
{
  r = _mm_mask_cvtss_sd (r, m, s1, s2);
  r = _mm_maskz_cvtss_sd (m, s1, s2);
  r = _mm_cvt_roundss_sd (s1, s2, _MM_FROUND_NO_EXC);
  r = _mm_mask_cvt_roundss_sd (r, m, s1, s2, _MM_FROUND_NO_EXC);
  r = _mm_maskz_cvt_roundss_sd (m, s1, s2, _MM_FROUND_NO_EXC);
}
