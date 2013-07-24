/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#define SIZE (128 / 64)
#include "avx512f-mask-type.h"

#include <math.h>
#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_vgetexpsd (double *s, double *r)
{
  r[0] = floor (log (s[0]) / log (2));
}

void static
avx512f_test (void)
{
  int i;
  union128d res1, res2, res3, s1;
  __mmask8 mask = MASK_VALUE;
  double res_ref[SIZE];

  for (i = 0; i < SIZE; i++)
    {
      s1.a[i] = 5.0 - i;
      res_ref[i] = s1.a[i];
    }

  res1.x = _mm_getexp_sd (s1.x, s1.x);
  res2.x = _mm_mask_getexp_sd (res2.x, mask, s1.x, s1.x);
  res3.x = _mm_maskz_getexp_sd (mask, s1.x, s1.x);

  compute_vgetexpsd (s1.a, res_ref);

  if (check_fp_union128d (res1, res_ref))
    abort ();

  MASK_MERGE (d) (res_ref, mask, 1);
  if (check_fp_union128d (res2, res_ref))
    abort ();

  MASK_ZERO (d) (res_ref, mask, 1);
  if (check_fp_union128d (res3, res_ref))
    abort ();
}
