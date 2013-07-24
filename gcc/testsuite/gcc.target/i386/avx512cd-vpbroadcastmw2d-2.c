/* { dg-do run } */
/* { dg-options "-O2 -mavx512cd -DHAVE_512 -DAVX512CD" } */
/* { dg-require-effective-target avx512cd } */

#include "avx512f-helper.h"

#define SIZE (AVX512F_LEN / 32)

CALC (int *res, __mmask16 src)
{
  int i;

  for (i = 0; i < SIZE; i++)
    res[i] = src;
}

static void
TEST (void)
{
  int i;
  UNION_TYPE (AVX512F_LEN, i_d) res;
  int res_ref[SIZE];
  __mmask16 src;

  for (i = 0; i < SIZE; i++)
    {
      res.a[i] = -1;
    }

  res.x = INTRINSIC (_broadcastmw_epi32) (src);

  CALC (res_ref, src);

  if (UNION_CHECK (AVX512F_LEN, i_d) (res, res_ref))
    abort ();
}
