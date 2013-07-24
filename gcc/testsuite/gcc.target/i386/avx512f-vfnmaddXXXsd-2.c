/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_vfnmaddsd (double *r, double *s1, double *s2, double *s3)
{
  r[0] = -s1[0] * s2[0] + s3[0];
}

static void
avx512f_test (void)
{
  int i;
  union128d res1, res2, res3, src1, src2;
  __mmask8 mask = MASK_VALUE;
  double res_ref1[2], res_ref2[2];

  for (i = 0; i < 2; i++)
    {
      res_ref1[i] = res_ref2[i] = DEFAULT_VALUE;
      res1.a[i] = res2.a[i] = res3.a[i] = DEFAULT_VALUE;
      src1.a[i] = i * i - 8.179;
      src2.a[i] = 2.45 - i;
    }

  compute_vfnmaddsd (res_ref1, res1.a, src1.a, src2.a);
  compute_vfnmaddsd (res_ref2, src1.a, src2.a, res2.a);

  res1.x = _mm_mask_fnmadd_sd (res1.x, mask, src1.x, src2.x);
  res2.x = _mm_mask3_fnmadd_sd (src1.x, src2.x, res2.x, mask);
  res3.x = _mm_maskz_fnmadd_sd (mask, res3.x, src1.x, src2.x);

  MASK_MERGE (d) (res_ref1, mask, 1);
  if (check_fp_union128d (res1, res_ref1))
    abort ();

  MASK_MERGE (d) (res_ref2, mask, 1);
  if (check_fp_union128d (res2, res_ref2))
    abort ();

  MASK_ZERO (d) (res_ref1, mask, 1);
  if (check_fp_union128d (res3, res_ref1))
    abort ();
}
