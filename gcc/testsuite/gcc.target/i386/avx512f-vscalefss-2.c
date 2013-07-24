/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#define SIZE (128 / 32)
#include "avx512f-mask-type.h"

#include <math.h>
#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_scalefss (float *s1, float *s2, float *r)
{
  r[0] = s1[0] * (float) pow (2, floor (s2[0]));
  r[1] = s1[1];
  r[2] = s1[2];
  r[3] = s1[3];
}

static void
avx512f_test (void)
{
  union128 res1, res2, res3, s1, s2;
  __mmask8 mask = MASK_VALUE;
  float res_ref[SIZE];
  int i;

  for (i = 0; i < SIZE; i++)
    {
      s1.a[i] = 11.5 * (i + 1);
      s2.a[i] = 10.5 * (i + 1);
      res2.a[i] = DEFAULT_VALUE;
    }

  res1.x = _mm_scalef_ss (s1.x, s2.x);
  res2.x = _mm_mask_scalef_ss (res2.x, mask, s1.x, s2.x);
  res3.x = _mm_maskz_scalef_ss (mask, s1.x, s2.x);

  compute_scalefss (s1.a, s2.a, res_ref);

  if (check_union128 (res1, res_ref))
    abort ();

  MASK_MERGE () (res_ref, mask, 1);
  if (check_union128 (res2, res_ref))
    abort ();

  MASK_ZERO () (res_ref, mask, 1);
  if (check_union128 (res3, res_ref))
    abort ();
}
