/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#include <math.h>
#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_vsqrtss (float *s1, float *s2, float *r)
{
  r[0] = sqrt (s2[0]);
  r[1] = s1[1];
  r[2] = s1[2];
  r[3] = s1[3];
}

static void
avx512f_test (void)
{
  union128 s1, s2, res1, res2;
  __mmask8 mask = MASK_VALUE;
  float res_ref[4];

  s1.x = _mm_set_ps (-24.43, 68.346, -43.35, 546.46);
  s2.x = _mm_set_ps (222.222, 333.333, 444.444, 4.0);
  res1.a[0] = DEFAULT_VALUE;

  res1.x = _mm_mask_sqrt_ss (res1.x, mask, s1.x, s2.x);
  res2.x = _mm_maskz_sqrt_ss (mask, s1.x, s2.x);

  compute_vsqrtss (s1.a, s2.a, res_ref);

  MASK_MERGE () (res_ref, mask, 1);
  if (check_fp_union128 (res1, res_ref))
    abort ();

  MASK_ZERO () (res_ref, mask, 1);
  if (check_fp_union128 (res2, res_ref))
    abort ();
}
