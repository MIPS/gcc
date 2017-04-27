/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -march=armv8-a+sve -fno-inline" } */

#include "sve_vcond_2.C"

#include <stdlib.h>

#define TEST_VCOND(DATA_TYPE, CMP_TYPE, COND, SUFFIX)		\
{								\
  const int n = 32 / sizeof (DATA_TYPE);			\
  CMP_TYPE x[n], y[n];						\
  DATA_TYPE a[n], b[n];						\
  for (int i = 0; i < n; ++i)					\
    {								\
      x[i] = i;							\
      y[i] = (i & 1) + 5;					\
      a[i] = 6 * i;						\
      b[i] = 4 + i;						\
    }								\
  vcond_##CMP_TYPE##SUFFIX (r_##DATA_TYPE, a, b, x, y, n);	\
  for (int i = 0; i < n; ++i)					\
    if (r_##DATA_TYPE[i] != (x[i] COND y[i] ? a[i] : b[i]))	\
      abort ();							\
}

#define TEST_VCOND_IMM(DATA_TYPE, CMP_TYPE, COND, IMM, SUFFIX)	\
{								\
  const int n = 32 / sizeof (DATA_TYPE);			\
  CMP_TYPE x[n];						\
  DATA_TYPE a[n], b[n];						\
  for (int i = 0; i < n; ++i)					\
    {								\
      x[i] = i - 1;						\
      a[i] = 5 * i + IMM;					\
      b[i] = 7 + i - IMM * 2;					\
    }								\
  vcond_imm_##CMP_TYPE##SUFFIX (r_##DATA_TYPE, a, b, x, n);	\
  for (int i = 0; i < n; ++i)					\
    if (r_##DATA_TYPE[i] != (x[i] COND IMM ? a[i] : b[i]))	\
      abort ();							\
}

#define TEST_VCOND_SIGNED_ALL(COND, SUFFIX)		\
  TEST_VCOND (int8_t, int8_t, COND, SUFFIX)		\
  TEST_VCOND (int16_t, int16_t, COND, SUFFIX)		\
  TEST_VCOND (int32_t, int32_t, COND, SUFFIX)		\
  TEST_VCOND (int64_t, int64_t, COND, SUFFIX)		\
  TEST_VCOND (float, int32_t, COND, SUFFIX##_float)	\
  TEST_VCOND (double, int64_t, COND, SUFFIX##_double)

#define TEST_VCOND_UNSIGNED_ALL(COND, SUFFIX)		\
  TEST_VCOND (uint8_t, uint8_t, COND, SUFFIX)		\
  TEST_VCOND (uint16_t, uint16_t, COND, SUFFIX)		\
  TEST_VCOND (uint32_t, uint32_t, COND, SUFFIX)		\
  TEST_VCOND (uint64_t, uint64_t, COND, SUFFIX)		\
  TEST_VCOND (float, uint32_t, COND, SUFFIX##_float)	\
  TEST_VCOND (double, uint64_t, COND, SUFFIX##_double)

#define TEST_VCOND_ALL(COND, SUFFIX)		\
  TEST_VCOND_SIGNED_ALL (COND, SUFFIX)		\
  TEST_VCOND_UNSIGNED_ALL (COND, SUFFIX)

#define TEST_VCOND_IMM_SIGNED_ALL(COND, IMM, SUFFIX)		\
  TEST_VCOND_IMM (int8_t, int8_t, COND, IMM, SUFFIX)		\
  TEST_VCOND_IMM (int16_t, int16_t, COND, IMM, SUFFIX)		\
  TEST_VCOND_IMM (int32_t, int32_t, COND, IMM, SUFFIX)		\
  TEST_VCOND_IMM (int64_t, int64_t, COND, IMM, SUFFIX)		\
  TEST_VCOND_IMM (float, int32_t, COND, IMM, SUFFIX##_float)	\
  TEST_VCOND_IMM (double, int64_t, COND, IMM, SUFFIX##_double)

#define TEST_VCOND_IMM_UNSIGNED_ALL(COND, IMM, SUFFIX)		\
  TEST_VCOND_IMM (uint8_t, uint8_t, COND, IMM, SUFFIX)		\
  TEST_VCOND_IMM (uint16_t, uint16_t, COND, IMM, SUFFIX)	\
  TEST_VCOND_IMM (uint32_t, uint32_t, COND, IMM, SUFFIX)	\
  TEST_VCOND_IMM (uint64_t, uint64_t, COND, IMM, SUFFIX)	\
  TEST_VCOND_IMM (float, uint32_t, COND, IMM, SUFFIX##_float)	\
  TEST_VCOND_IMM (double, uint64_t, COND, IMM, SUFFIX##_double)

#define TEST_VCOND_IMM_ALL(COND, IMM, SUFFIX)	\
  TEST_VCOND_IMM_SIGNED_ALL (COND, IMM, SUFFIX)	\
  TEST_VCOND_IMM_UNSIGNED_ALL (COND, IMM, SUFFIX)

#define DEF_INIT_VECTOR(TYPE)			\
  TYPE r_##TYPE[NUM_ELEMS(TYPE)];		\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    r_##TYPE[i] = i * 3;

int __attribute__ ((optimize (1)))
main (int argc, char **argv)
{
  int result = 0;
  DEF_INIT_VECTOR (int8_t)
  DEF_INIT_VECTOR (int16_t)
  DEF_INIT_VECTOR (int32_t)
  DEF_INIT_VECTOR (int64_t)
  DEF_INIT_VECTOR (uint8_t)
  DEF_INIT_VECTOR (uint16_t)
  DEF_INIT_VECTOR (uint32_t)
  DEF_INIT_VECTOR (uint64_t)
  DEF_INIT_VECTOR (float)
  DEF_INIT_VECTOR (double)

  TEST_VCOND_ALL (>, _gt)
  TEST_VCOND_ALL (<, _lt)
  TEST_VCOND_ALL (>=, _ge)
  TEST_VCOND_ALL (<=, _le)
  TEST_VCOND_ALL (==, _eq)
  TEST_VCOND_ALL (!=, _ne)

  TEST_VCOND_IMM_ALL (>, 5, _gt)
  TEST_VCOND_IMM_ALL (<, 5, _lt)
  TEST_VCOND_IMM_ALL (>=, 5, _ge)
  TEST_VCOND_IMM_ALL (<=, 5, _le)
  TEST_VCOND_IMM_ALL (==, 5, _eq)
  TEST_VCOND_IMM_ALL (!=, 5, _ne)

  return 0;
}
