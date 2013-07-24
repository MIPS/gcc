/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_maxss (float * src1, float *src2, float *dst)
{
  int i;
  dst[0] = src1[0] > src2[0] ? src1[0]: src2[0];
  for (i = 1; i < 4; i++)
    dst[i] = src1[i];
}

void static
avx512f_test (void)
{
  int i;
  union128 src1, src2, res1, res2;
  __mmask8 mask = MASK_VALUE;
  float res_ref[4];

  for (i = 0; i < 4; i++)
    {
      src1.a[i] = 1. / (i + 1);
      src2.a[i] = i;
    }

  res1.a[0] = DEFAULT_VALUE;

  res1.x = _mm_mask_max_ss (res1.x, mask, src1.x, src2.x);
  res2.x = _mm_maskz_max_ss (mask, src1.x, src2.x);

  compute_maxss (src1.a, src2.a, res_ref);

  MASK_MERGE () (res_ref, mask, 1);
  if (check_union128 (res1, res_ref))
    abort ();

  MASK_ZERO () (res_ref, mask, 1);
  if (check_union128 (res2, res_ref))
    abort ();
}
