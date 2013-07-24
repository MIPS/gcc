/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_maxsd (double * src1, double *src2, double *dst)
{
  int i;
  dst[0] = src1[0] > src2[0] ? src1[0]: src2[0];
  dst[1] = src1[1];
}

void static
avx512f_test (void)
{
  int i;
  union128d src1, src2, res1, res2;
  __mmask8 mask = MASK_VALUE;
  double res_ref[2];

  for (i = 0; i < 2; i++)
    {
      src1.a[i] = 1. / (i + 1);
      src2.a[i] = i;
    }

  res1.a[0] = DEFAULT_VALUE;

  res1.x = _mm_mask_max_sd (res1.x, mask, src1.x, src2.x);
  res2.x = _mm_maskz_max_sd (mask, src1.x, src2.x);

  compute_maxsd (src1.a, src2.a, res_ref);

  MASK_MERGE (d) (res_ref, mask, 1);
  if (check_union128d (res1, res_ref))
    abort ();

  MASK_ZERO (d) (res_ref, mask, 1);
  if (check_union128d (res2, res_ref))
    abort ();
}
