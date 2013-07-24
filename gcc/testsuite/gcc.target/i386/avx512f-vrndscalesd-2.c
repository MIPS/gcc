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
compute_rndscalesd (double *s1, double *s2, double *r, int imm)
{
  int rc, m;
  rc = imm & 0xf;
  m = imm >> 4;

  switch (rc)
    {
    case _MM_FROUND_FLOOR:
      r[0] = floor (s2[0] * pow (2, m)) / pow (2, m);
      break;
    case _MM_FROUND_CEIL:
      r[0] = ceil (s2[0] * pow (2, m)) / pow (2, m);
      break;
    default:
      abort ();
      break;
    }

  r[1] = s1[1];
}

static void
avx512f_test (void)
{
  int imm = _MM_FROUND_FLOOR | (7 << 4);
  union128d s1, s2, res1, res2, res3;
  __mmask8 mask = MASK_VALUE;
  double res_ref[SIZE];

  s1.x = _mm_set_pd (4.05084, -1.23162);
  s2.x = _mm_set_pd (-3.53222, 7.33527);

  res1.x = _mm_roundscale_sd (s1.x, s2.x, imm);
  res2.x = _mm_mask_roundscale_sd (res2.x, mask, s1.x, s2.x, imm);
  res3.x = _mm_maskz_roundscale_sd (mask, s1.x, s2.x, imm);

  compute_rndscalesd (s1.a, s2.a, res_ref, imm);

  if (check_union128d (res1, res_ref))
    abort ();

  MASK_MERGE (d) (res_ref, mask, 1);
  if (check_union128d (res2, res_ref))
    abort ();

  MASK_ZERO (d) (res_ref, mask, 1);
  if (check_union128d (res3, res_ref))
    abort ();
}
