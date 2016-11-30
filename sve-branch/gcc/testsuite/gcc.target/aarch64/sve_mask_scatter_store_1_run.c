/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <unistd.h>
#include <stdio.h>

extern void abort (void);
extern void *memset(void *, int, size_t);

#include "sve_mask_scatter_store_1.c"

#define NUM_SRC_ELEMS 13
#define NUM_DST_ELEMS(STRIDE) (NUM_SRC_ELEMS * STRIDE)

#define MASKED_VALUE 3

#define TEST_MASK_SCATTER_STORE_COMMON1(FUN,OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  MASKTYPE masks[NUM_DST_ELEMS (STRIDE)];\
  memset (masks, 0, (NUM_DST_ELEMS (STRIDE)) * sizeof (MASKTYPE));\
  real_src[0] = 0;\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    {\
      src[i] = i;\
      masks[i * STRIDE] = i & 0x1;\
    }\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS (STRIDE); i++)\
    dst[i] = MASKED_VALUE;\
  FUN##OBJTYPE##STRIDETYPE##STRIDE (dst, src, masks, NUM_SRC_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    if (dst[i * STRIDE] != (masks[i * STRIDE] ? i : MASKED_VALUE))\
      abort ();\
}

#define TEST_MASK_SCATTER_STORE_COMMON2(FUN,OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  MASKTYPE masks[NUM_DST_ELEMS (STRIDE)];\
  memset (masks, 0, (NUM_DST_ELEMS (STRIDE)) * sizeof (MASKTYPE));\
  real_src[0] = 0;\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    {\
      src[i] = i;\
      masks[i * STRIDE] = i & 0x1;\
    }\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS (STRIDE); i++)\
    dst[i] = MASKED_VALUE;\
  FUN##OBJTYPE##STRIDETYPE (dst, src, masks, STRIDE, NUM_SRC_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    if (dst[i * STRIDE] != (masks[i * STRIDE] ? i : MASKED_VALUE))\
      abort ();\
}

#define TEST_MASK_SCATTER_STORE1(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_SCATTER_STORE_COMMON1 (mscatter_store1, OBJTYPE, MASKTYPE, \
				   STRIDETYPE, STRIDE)

#define TEST_MASK_SCATTER_STORE2(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_SCATTER_STORE_COMMON2 (mscatter_store2, OBJTYPE, MASKTYPE, \
				   STRIDETYPE, STRIDE)

#define TEST_MASK_SCATTER_STORE3(OBJTYPE,MASKTYPE,STRIDETYPE)\
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
  MASKTYPE masks[NUM_DST_ELEMS (5)];\
  memset (masks, 0, (NUM_DST_ELEMS (5)) * sizeof (MASKTYPE));\
  real_src1[0] = real_src2[0] = real_src3[0] = real_src4[0] = real_src5[0] = 0;\
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
      masks[i * 5] = i & 0x1;\
    }\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS (5); i++)\
    dst[i] = MASKED_VALUE;\
  mscatter_store3s5##OBJTYPE##STRIDETYPE \
    (dst, src1, src2, src3, src4, src5, masks, NUM_SRC_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS; i++)\
    {\
      STRIDETYPE base = i * 5;\
      if (dst[base] != (masks[i * 5] ? base : MASKED_VALUE))\
	abort ();\
      if (dst[base + 1] != (masks[i * 5] ? (base + 1) : MASKED_VALUE))\
	abort ();\
      if (dst[base + 2] != (masks[i * 5] ? (base + 2) : MASKED_VALUE))\
	abort ();\
      if (dst[base + 3] != (masks[i * 5] ? (base + 3) : MASKED_VALUE))\
	abort ();\
      if (dst[base + 4] != (masks[i * 5] ? (base + 4) : MASKED_VALUE))\
	abort ();\
    }\
}

#define TEST_MASK_SCATTER_STORE4(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_SCATTER_STORE_COMMON1 (mscatter_store4, OBJTYPE, MASKTYPE, \
				   STRIDETYPE, STRIDE)

#define TEST_MASK_SCATTER_STORE5(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_SCATTER_STORE_COMMON2 (mscatter_store5, OBJTYPE, MASKTYPE, \
				   STRIDETYPE, STRIDE)

int __attribute__ ((optimize (1)))
main ()
{
  TEST_MASK_SCATTER_STORE1 (double, long, long, 5);

  TEST_MASK_SCATTER_STORE1 (double, long, long, 8);
  TEST_MASK_SCATTER_STORE1 (double, long, long, 21);

  TEST_MASK_SCATTER_STORE1 (float, int, int, 5);
  TEST_MASK_SCATTER_STORE1 (float, int, int, 8);
  TEST_MASK_SCATTER_STORE1 (float, int, int, 21);

  TEST_MASK_SCATTER_STORE2 (double, long, long, 5);
  TEST_MASK_SCATTER_STORE2 (double, long, long, 8);
  TEST_MASK_SCATTER_STORE2 (double, long, long, 21);

  TEST_MASK_SCATTER_STORE2 (float, int, int, 5);
  TEST_MASK_SCATTER_STORE2 (float, int, int, 8);
  TEST_MASK_SCATTER_STORE2 (float, int, int, 21);

  TEST_MASK_SCATTER_STORE3 (double, long, long);
  TEST_MASK_SCATTER_STORE3 (float, int, int);

  TEST_MASK_SCATTER_STORE4 (double, long, long, 5);

  TEST_MASK_SCATTER_STORE5 (double, long, long, 5);
  TEST_MASK_SCATTER_STORE5 (float, int, int, 5);

  /* Widened forms.  */
  TEST_MASK_SCATTER_STORE1 (double, long, int, 5)
  TEST_MASK_SCATTER_STORE1 (double, long, int, 8)
  TEST_MASK_SCATTER_STORE1 (double, long, short, 5)
  TEST_MASK_SCATTER_STORE1 (double, long, short, 8)

  TEST_MASK_SCATTER_STORE1 (float, int, short, 5)
  TEST_MASK_SCATTER_STORE1 (float, int, short, 8)

  TEST_MASK_SCATTER_STORE2 (double, long, int, 5);
  TEST_MASK_SCATTER_STORE2 (double, long, int, 8);
  TEST_MASK_SCATTER_STORE2 (double, long, int, 21);

  TEST_MASK_SCATTER_STORE2 (float, int, short, 5);
  TEST_MASK_SCATTER_STORE2 (float, int, short, 8);
  TEST_MASK_SCATTER_STORE2 (float, int, short, 21);

  TEST_MASK_SCATTER_STORE4 (double, long, int, 5);
  TEST_MASK_SCATTER_STORE4 (float, int, short, 5);

  TEST_MASK_SCATTER_STORE5 (double, long, int, 5);

  return 0;
}

