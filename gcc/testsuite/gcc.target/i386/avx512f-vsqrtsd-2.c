/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#include <math.h>
#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_vsqrtsd (double *s1, double *s2, double *r)
{
  r[0] = sqrt (s2[0]);
  r[1] = s1[1];
}

static void
avx512f_test (void)
{
  union128d s1, s2, res1, res2;
  __mmask8 mask = MASK_VALUE;
  double res_ref[2];

  s1.x = _mm_set_pd (-3.0, 111.111);
  s2.x = _mm_set_pd (222.222, 4.0);
  res1.a[0] = DEFAULT_VALUE;

  res1.x = _mm_mask_sqrt_sd (res1.x, mask, s1.x, s2.x);
  res2.x = _mm_maskz_sqrt_sd (mask, s1.x, s2.x);

  compute_vsqrtsd (s1.a, s2.a, res_ref);

  MASK_MERGE (d) (res_ref, mask, 1);
  if (check_fp_union128d (res1, res_ref))
    abort ();

  MASK_ZERO (d) (res_ref, mask, 1);
  if (check_fp_union128d (res2, res_ref))
    abort ();
}
