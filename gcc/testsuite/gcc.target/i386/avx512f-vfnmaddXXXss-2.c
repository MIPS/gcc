/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_vfnmaddss (float *r, float *s1, float *s2, float *s3)
{
  r[0] = -s1[0] * s2[0] + s3[0];
}

static void
avx512f_test (void)
{
  int i;
  union128 res1, res2, res3, src1, src2;
  __mmask8 mask = MASK_VALUE;
  float res_ref1[4], res_ref2[4];

  for (i = 0; i < 4; i++)
    {
      res_ref1[i] = res_ref2[i] = DEFAULT_VALUE;
      res1.a[i] = res2.a[i] = res3.a[i] = DEFAULT_VALUE;
      src1.a[i] = i * i - 8.179;
      src2.a[i] = 2.45 - i;
    }

  compute_vfnmaddss (res_ref1, res1.a, src1.a, src2.a);
  compute_vfnmaddss (res_ref2, src1.a, src2.a, res2.a);

  res1.x = _mm_mask_fnmadd_ss (res1.x, mask, src1.x, src2.x);
  res2.x = _mm_mask3_fnmadd_ss (src1.x, src2.x, res2.x, mask);
  res3.x = _mm_maskz_fnmadd_ss (mask, res3.x, src1.x, src2.x);

  MASK_MERGE ()(res_ref1, mask, 1);
  if (check_fp_union128 (res1, res_ref1))
    abort ();

  MASK_MERGE ()(res_ref2, mask, 1);
  if (check_fp_union128 (res2, res_ref2))
    abort ();

  MASK_ZERO ()(res_ref1, mask, 1);
  if (check_fp_union128 (res3, res_ref1))
    abort ();
}
