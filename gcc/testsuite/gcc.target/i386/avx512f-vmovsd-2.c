/* { dg-do run } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-require-effective-target avx512f } */

#define SIZE (128 / 64)
#include "avx512f-mask-type.h"

#include "avx512f-check.h"
#include "avx512f-mask-type.h"
#include "avx512f-helper.h"

static void
compute_movesd (double *s1, double *s2, double *r)
{
  r[0] = s2[0];
  r[1] = s1[1];
}

static void
compute_loadsd (double *s, double *r)
{
  r[0] = s[0];
  r[1] = 0.0;
}

static void
compute_storesd (double *res, double *s, double *r)
{
  r[0] = s[0];
  r[1] = res[1];
}

static void
avx512f_test (void)
{
  union128d s1, s2, s4, res1, res2, res3, res4;
  __mmask8 mask = MASK_VALUE;
  double s3[SIZE];
  double res5[SIZE] = { 0.0 };
  double res_ref1[SIZE];
  double res_ref2[SIZE];
  double res_ref3[SIZE];
  int i, sign = 1;

  for (i = 0; i < SIZE; i++)
    {
      s1.a[i] = 12.34 * (i + 1) * sign;
      s2.a[i] = 56.78 * (i + 2) * sign;
      s3[i] = 9000.12 * (i + 3) * sign;
      s4.a[i] = 34.56 * (i + 4) * sign;
      sign = -sign;
    }

  res1.a[0] = DEFAULT_VALUE;
  res3.a[0] = DEFAULT_VALUE;
  res5[0] = DEFAULT_VALUE;

  res1.x = _mm_mask_move_sd (res1.x, mask, s1.x, s2.x);
  res2.x = _mm_maskz_move_sd (mask, s1.x, s2.x);
  res3.x = _mm_mask_load_sd (res3.x, mask, s3);
  res4.x = _mm_maskz_load_sd (mask, s3);
  _mm_mask_store_sd (res5, mask, s4.x);

  compute_movesd (s1.a, s2.a, res_ref1);
  compute_loadsd (s3, res_ref2);
  compute_storesd (res5, s4.a, res_ref3);

  MASK_MERGE (d) (res_ref1, mask, 1);
  if (check_union128d (res1, res_ref1))
    abort ();

  MASK_ZERO (d) (res_ref1, mask, 1);
  if (check_union128d (res2, res_ref1))
    abort ();

  MASK_MERGE (d) (res_ref2, mask, 1);
  if (checkVd (res3.a, res_ref2, SIZE))
    abort ();

  MASK_ZERO (d) (res_ref2, mask, 1);
  if (checkVd (res4.a, res_ref2, SIZE))
    abort ();

  MASK_MERGE (d) (res_ref3, mask, 1);
  if (checkVd (res_ref3, res5, SIZE))
    abort ();
}
