/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#define SIZE (128 / 32)
#include "avx512f-mask-type.h"

#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_vcvtsd2ss (float *s1, double *s2, float *r)
{
  r[0] = (float) s2[0];
  r[1] = s1[1];
  r[2] = s1[2];
  r[3] = s1[3];
}

static void
avx512f_test (void)
{
  union128 s1, res1, res2;
  union128d s2;
  __mmask8 mask = MASK_VALUE;
  float res_ref[SIZE];

  s1.x = _mm_set_ps (-24.43, 68.346, -43.35, 546.46);
  s2.x = _mm_set_pd (4560.987, -2301.987);
  res1.a[0] = DEFAULT_VALUE;

  res1.x = _mm_mask_cvtsd_ss (res1.x, mask, s1.x, s2.x);
  res2.x = _mm_maskz_cvtsd_ss (mask, s1.x, s2.x);

  compute_vcvtsd2ss (s1.a, s2.a, res_ref);

  MASK_MERGE () (res_ref, mask, 1);
  if (check_union128 (res1, res_ref))
    abort ();

  MASK_ZERO () (res_ref, mask, 1);
  if (check_union128 (res2, res_ref))
    abort ();
}
