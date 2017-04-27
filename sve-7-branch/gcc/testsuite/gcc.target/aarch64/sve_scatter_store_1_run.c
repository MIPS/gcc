/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <unistd.h>

extern void abort (void);
extern void *memset(void *, int, size_t);

#include "sve_scatter_store_1.c"

#define NUM_SRC_ELEMS 13
#define NUM_DST_ELEMS(STRIDE) (NUM_SRC_ELEMS * STRIDE)

#define TEST_SCATTER_STORE_COMMON1(FUN,OBJTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  memset (real_src, 0, (1 + NUM_SRC_ELEMS) * sizeof (OBJTYPE));\
  memset (real_dst, 0, (1 + NUM_DST_ELEMS (STRIDE)) * sizeof (OBJTYPE));\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    src[i] = i;\
  FUN##OBJTYPE##STRIDETYPE##STRIDE (dst, src, NUM_SRC_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    if (dst[i * STRIDE] != i)\
      abort ();\
}

#define TEST_SCATTER_STORE_COMMON2(FUN,OBJTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  memset (real_src, 0, (1 + NUM_SRC_ELEMS) * sizeof (OBJTYPE));\
  memset (real_dst, 0, (1 + NUM_DST_ELEMS (STRIDE)) * sizeof (OBJTYPE));\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    src[i] = i;\
  FUN##OBJTYPE##STRIDETYPE (dst, src, STRIDE, NUM_SRC_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    if (dst[i * STRIDE] != i)\
      abort ();\
}

#define TEST_SCATTER_STORE1(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_SCATTER_STORE_COMMON1 (scatter_store1, OBJTYPE, STRIDETYPE, STRIDE)

#define TEST_SCATTER_STORE2(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_SCATTER_STORE_COMMON2 (scatter_store2, OBJTYPE, STRIDETYPE, STRIDE)

#define TEST_SCATTER_STORE3(OBJTYPE,STRIDETYPE)\
{\
  OBJTYPE real_src1[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_src2[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_src3[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_src4[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_src5[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS (5)]\
    __attribute__((aligned (32)));\
  memset (real_src1, 0, (1 + NUM_SRC_ELEMS) * sizeof (OBJTYPE));\
  memset (real_src2, 0, (1 + NUM_SRC_ELEMS) * sizeof (OBJTYPE));\
  memset (real_src3, 0, (1 + NUM_SRC_ELEMS) * sizeof (OBJTYPE));\
  memset (real_src4, 0, (1 + NUM_SRC_ELEMS) * sizeof (OBJTYPE));\
  memset (real_src5, 0, (1 + NUM_SRC_ELEMS) * sizeof (OBJTYPE));\
  memset (real_dst, 0, (1 + NUM_DST_ELEMS (5)) * sizeof (OBJTYPE));\
  OBJTYPE *src1 = &real_src1[1];\
  OBJTYPE *src2 = &real_src2[1];\
  OBJTYPE *src3 = &real_src3[1];\
  OBJTYPE *src4 = &real_src4[1];\
  OBJTYPE *src5 = &real_src5[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    {\
      STRIDETYPE base = i * 5;\
      src1[i] = base;\
      src2[i] = base + 1;\
      src3[i] = base + 2;\
      src4[i] = base + 3;\
      src5[i] = base + 4;\
    }\
  scatter_store3s5##OBJTYPE##STRIDETYPE \
    (dst, src1, src2, src3, src4, src5, NUM_SRC_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS (5); i++)\
    if (dst[i] != i)\
      abort ();\
}

#define TEST_SCATTER_STORE4(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_SCATTER_STORE_COMMON1 (scatter_store4, OBJTYPE, STRIDETYPE, STRIDE)

#define TEST_SCATTER_STORE5(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_SCATTER_STORE_COMMON2 (scatter_store5, OBJTYPE, STRIDETYPE, STRIDE)

int __attribute__ ((optimize (1)))
main ()
{
  TEST_SCATTER_STORE1 (double, long, 5);
  TEST_SCATTER_STORE1 (double, long, 8);
  TEST_SCATTER_STORE1 (double, long, 21);

  TEST_SCATTER_STORE1 (float, int, 5);
  TEST_SCATTER_STORE1 (float, int, 8);
  TEST_SCATTER_STORE1 (float, int, 21);

  TEST_SCATTER_STORE2 (double, long, 5);
  TEST_SCATTER_STORE2 (double, long, 8);
  TEST_SCATTER_STORE2 (double, long, 21);

  TEST_SCATTER_STORE2 (float, int, 5);
  TEST_SCATTER_STORE2 (float, int, 8);
  TEST_SCATTER_STORE2 (float, int, 21);

  TEST_SCATTER_STORE3 (double, long);
  TEST_SCATTER_STORE3 (float, int);

  TEST_SCATTER_STORE4 (double, long, 5);

  TEST_SCATTER_STORE5 (double, long, 5);
  TEST_SCATTER_STORE5 (float, int, 5);

  /* Widened forms.  */
  TEST_SCATTER_STORE1 (double, int, 5)
  TEST_SCATTER_STORE1 (double, int, 8)
  TEST_SCATTER_STORE1 (double, short, 5)
  TEST_SCATTER_STORE1 (double, short, 8)

  TEST_SCATTER_STORE1 (float, short, 5)
  TEST_SCATTER_STORE1 (float, short, 8)

  TEST_SCATTER_STORE2 (double, int, 5);
  TEST_SCATTER_STORE2 (double, int, 8);
  TEST_SCATTER_STORE2 (double, int, 21);

  TEST_SCATTER_STORE2 (float, short, 5);
  TEST_SCATTER_STORE2 (float, short, 8);
  TEST_SCATTER_STORE2 (float, short, 21);

  TEST_SCATTER_STORE4 (double, int, 5);
  TEST_SCATTER_STORE4 (float, short, 5);

  TEST_SCATTER_STORE5 (double, int, 5);

  return 0;
}

