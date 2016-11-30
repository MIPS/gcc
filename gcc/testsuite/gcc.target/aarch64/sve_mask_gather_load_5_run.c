/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <unistd.h>

extern void abort (void);
extern void *memset(void *, int, size_t);

#include "sve_mask_gather_load_5.c"

#define NUM_DST_ELEMS 13
#define NUM_SRC_ELEMS(STRIDE) (NUM_DST_ELEMS * STRIDE)

#define MASKED_VALUE 3

#define TEST_MASK_GATHER_LOAD_COMMON1(FUN,OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  MASKTYPE masks[NUM_SRC_ELEMS (STRIDE)];\
  memset (real_src, 0, (1 + NUM_SRC_ELEMS (STRIDE)) * sizeof (OBJTYPE));\
  memset (masks, 0, (NUM_SRC_ELEMS (STRIDE)) * sizeof (MASKTYPE));\
  real_dst[0] = 0;\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    {\
      src[i * STRIDE] = i;\
      dst[i] = MASKED_VALUE;\
      masks[i * STRIDE] = i & 0x1;\
    }\
  FUN##OBJTYPE##STRIDETYPE##STRIDE \
    (dst, src, masks, NUM_DST_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    if (dst[i] != (masks[i * STRIDE] ? i : MASKED_VALUE))\
      abort ();\
}

#define TEST_MASK_GATHER_LOAD_COMMON2(FUN,OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  MASKTYPE masks[NUM_SRC_ELEMS (STRIDE)];\
  memset (real_src, 0, (1 + NUM_SRC_ELEMS (STRIDE)) * sizeof (OBJTYPE));\
  memset (masks, 0, (NUM_SRC_ELEMS (STRIDE)) * sizeof (MASKTYPE));\
  real_dst[0] = 0;\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    {\
      src[i * STRIDE] = i;\
      dst[i] = MASKED_VALUE;\
      masks[i * STRIDE] = i & 0x1;\
    }\
  FUN##OBJTYPE##STRIDETYPE \
    (dst, src, masks, STRIDE, NUM_DST_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    if (dst[i] != (masks[i * STRIDE] ? i : MASKED_VALUE))\
      abort ();\
}

#define TEST_MASK_GATHER_LOAD1(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_GATHER_LOAD_COMMON1 (mgather_load1, OBJTYPE, MASKTYPE, \
				 STRIDETYPE, STRIDE)

#define TEST_MASK_GATHER_LOAD2(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_GATHER_LOAD_COMMON2 (mgather_load2, OBJTYPE, MASKTYPE, \
				 STRIDETYPE, STRIDE)

#define TEST_MASK_GATHER_LOAD3(OBJTYPE,MASKTYPE,STRIDETYPE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS (5)]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst1[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst2[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst3[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst4[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst5[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  MASKTYPE masks[NUM_SRC_ELEMS (5)];\
  memset (real_src, 0, (1 + NUM_SRC_ELEMS (5)) * sizeof (OBJTYPE));\
  memset (masks, 0, (NUM_SRC_ELEMS (5)) * sizeof (MASKTYPE));\
  real_dst1[0] = real_dst2[0] = real_dst3[0] = real_dst4[0] = real_dst5[0] = 0;\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst1 = &real_dst1[1];\
  OBJTYPE *dst2 = &real_dst2[1];\
  OBJTYPE *dst3 = &real_dst3[1];\
  OBJTYPE *dst4 = &real_dst4[1];\
  OBJTYPE *dst5 = &real_dst5[1];\
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS (5); i++)\
    src[i] = i;\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    {\
      dst1[i] = MASKED_VALUE;\
      dst2[i] = MASKED_VALUE;\
      dst3[i] = MASKED_VALUE;\
      dst4[i] = MASKED_VALUE;\
      dst5[i] = MASKED_VALUE;\
      masks[i * 5] = i & 0x1;\
    }\
  mgather_load3s5##OBJTYPE##STRIDETYPE \
    (dst1, dst2, dst3, dst4, dst5, src, masks, NUM_DST_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    {\
      STRIDETYPE base = i * 5;\
      if (dst1[i] != (masks[base] ? base : MASKED_VALUE))\
	abort ();\
      if (dst2[i] != (masks[base] ? (base + 1) : MASKED_VALUE))\
	abort ();\
      if (dst3[i] != (masks[base] ? (base + 2) : MASKED_VALUE))\
	abort ();\
      if (dst4[i] != (masks[base] ? (base + 3) : MASKED_VALUE))\
	abort ();\
      if (dst5[i] != (masks[base] ? (base + 4) : MASKED_VALUE))\
	abort ();\
    }\
}

#define TEST_MASK_GATHER_LOAD4(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_GATHER_LOAD_COMMON1 (mgather_load4, OBJTYPE, MASKTYPE, \
				 STRIDETYPE, STRIDE)

#define TEST_MASK_GATHER_LOAD5(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE) \
  TEST_MASK_GATHER_LOAD_COMMON2 (mgather_load5, OBJTYPE, MASKTYPE, \
				 STRIDETYPE, STRIDE)

int main ()
{
  TEST_MASK_GATHER_LOAD1 (double, long, long, 5);
  TEST_MASK_GATHER_LOAD1 (double, long, long, 8);
  TEST_MASK_GATHER_LOAD1 (double, long, long, 21);

  TEST_MASK_GATHER_LOAD1 (float, int, int, 5);
  TEST_MASK_GATHER_LOAD1 (float, int, int, 8);
  TEST_MASK_GATHER_LOAD1 (float, int, int, 21);

  TEST_MASK_GATHER_LOAD2 (double, long, long, 5);
  TEST_MASK_GATHER_LOAD2 (double, long, long, 8);
  TEST_MASK_GATHER_LOAD2 (double, long, long, 21);

  TEST_MASK_GATHER_LOAD3 (double, long, long);
  TEST_MASK_GATHER_LOAD3 (float, int, int);

  TEST_MASK_GATHER_LOAD4 (double, long, long, 5);

  TEST_MASK_GATHER_LOAD5 (double, long, long, 5);
  TEST_MASK_GATHER_LOAD5 (float, int, int, 5);

  /* Widened forms.  */
  TEST_MASK_GATHER_LOAD1 (double, long, int, 5)
  TEST_MASK_GATHER_LOAD1 (double, long, int, 8)
  TEST_MASK_GATHER_LOAD1 (double, long, short, 5)
  TEST_MASK_GATHER_LOAD1 (double, long, short, 8)

  TEST_MASK_GATHER_LOAD1 (float, int, short, 5)
  TEST_MASK_GATHER_LOAD1 (float, int, short, 8)

  TEST_MASK_GATHER_LOAD2 (double, long, int, 5);
  TEST_MASK_GATHER_LOAD2 (double, long, int, 8);
  TEST_MASK_GATHER_LOAD2 (double, long, int, 21);

  TEST_MASK_GATHER_LOAD4 (double, long, int, 5);
  TEST_MASK_GATHER_LOAD4 (float, int, short, 5);

  TEST_MASK_GATHER_LOAD5 (double, long, int, 5);

  return 0;
}

