/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-inline -march=armv8-a+sve" } */

#include "sve_vec_bool_cmp_1.c"

extern void abort (void);

#define N 103

#define TEST_VEC_BOOL_CMPNE(VARTYPE,INDUCTYPE)			\
{								\
  INDUCTYPE i;							\
  VARTYPE src[N];						\
  VARTYPE dst[N];						\
  for (i = 0; i < N; i++)					\
    {								\
      src[i] = i;						\
      dst[i] = i * 2;						\
    }								\
  vec_bool_cmpne##VARTYPE##INDUCTYPE (dst, src, 13, 97, 0xFF);	\
  for (i = 0; i < 13; i++)					\
    if (dst[i] != i)						\
      abort ();							\
  for (i = 13; i < N; i++)					\
    if (i != 0x3D && dst[i] != (i * 2))				\
      abort ();							\
    else if (i == 0x3D && dst[i] != 0x3D)			\
      abort ();							\
}

#define TEST_VEC_BOOL_CMPEQ(VARTYPE,INDUCTYPE)			\
{								\
  INDUCTYPE i;							\
  VARTYPE src[N];						\
  VARTYPE dst[N];						\
  for (i = 0; i < N; i++)					\
    {								\
      src[i] = i;						\
      dst[i] = i * 2;						\
    }								\
  vec_bool_cmpeq##VARTYPE##INDUCTYPE (dst, src, 13, 97, 0xFF);	\
  for (i = 0; i < 13; i++)					\
    if (dst[i] != (i * 2))					\
      abort ();							\
  for (i = 13; i < 97; i++)					\
    if (i != 0x3D && dst[i] != i)				\
      abort ();							\
    else if (i == 0x3D && dst[i] != (0x3D) * 2)			\
      abort ();							\
  for (i = 97; i < N; i++)					\
    if (dst[i] != (i * 2))					\
      abort ();							\
}

int main ()
{
  TEST_VEC_BOOL_CMPNE (uint8_t, uint8_t);
  TEST_VEC_BOOL_CMPNE (uint16_t, uint16_t);
  TEST_VEC_BOOL_CMPNE (uint32_t, uint32_t);
  TEST_VEC_BOOL_CMPNE (uint64_t, uint64_t);
  TEST_VEC_BOOL_CMPNE (float, uint32_t);
  TEST_VEC_BOOL_CMPNE (double, uint64_t);

  TEST_VEC_BOOL_CMPEQ (uint8_t, uint8_t);
  TEST_VEC_BOOL_CMPEQ (uint16_t, uint16_t);
  TEST_VEC_BOOL_CMPEQ (uint32_t, uint32_t);
  TEST_VEC_BOOL_CMPEQ (uint64_t, uint64_t);
  TEST_VEC_BOOL_CMPEQ (float, uint32_t);
  TEST_VEC_BOOL_CMPEQ (double, uint64_t);

  return 0;
}
