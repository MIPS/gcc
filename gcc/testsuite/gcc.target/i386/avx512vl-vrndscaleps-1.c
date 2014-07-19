/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vrndscaleps\[ \\t\]+\[^\n\]*%ymm\[0-9\]" 9} } */
/* { dg-final { scan-assembler-times "vrndscaleps\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 3} } */
/* { dg-final { scan-assembler-times "vrndscaleps\[ \\t\]+\[^\n\]*%ymm\[0-9\]\{%k\[1-7\]\}\{z\}" 3} } */
/* { dg-final { scan-assembler-times "vrndscaleps\[ \\t\]+\[^\n\]*%xmm\[0-9\]" 9} } */
/* { dg-final { scan-assembler-times "vrndscaleps\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\[^\{\]" 3} } */
/* { dg-final { scan-assembler-times "vrndscaleps\[ \\t\]+\[^\n\]*%xmm\[0-9\]\{%k\[1-7\]\}\{z\}" 3} } */

#include <immintrin.h>

volatile __m256 x1;
volatile __m128 x2;

void extern
avx512vl_test (void)
{
  x1 = _mm256_roundscale_ps (x1, 0x42);
  x1 = _mm256_ceil_ps (x1);
  x1 = _mm256_floor_ps (x1);
  x1 = _mm256_mask_roundscale_ps (x1, 2, x1, 0x42);
  x1 = _mm256_mask_ceil_ps (x1, 2, x1);
  x1 = _mm256_mask_floor_ps (x1, 2, x1);
  x1 = _mm256_maskz_roundscale_ps (2, x1, 0x42);
  x1 = _mm256_maskz_ceil_ps (2, x1);
  x1 = _mm256_maskz_floor_ps (2, x1);
  x2 = _mm_roundscale_ps (x2, 0x42);
  x2 = _mm_ceil_ps (x2);
  x2 = _mm_floor_ps (x2);
  x2 = _mm_mask_roundscale_ps (x2, 2, x2, 0x42);
  x2 = _mm_mask_ceil_ps (x2, 2, x2);
  x2 = _mm_mask_floor_ps (x2, 2, x2);
  x2 = _mm_maskz_roundscale_ps (2, x2, 0x42);
  x2 = _mm_maskz_ceil_ps (2, x2);
  x2 = _mm_maskz_floor_ps (2, x2);
}
