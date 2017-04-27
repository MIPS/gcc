/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <unistd.h>

extern void abort (void);
extern void *memset(void *, int, size_t);

#include "sve_gather_load_5.c"

#define NUM_DST_ELEMS 13
#define NUM_SRC_ELEMS(STRIDE) (NUM_DST_ELEMS * STRIDE)

#define TEST_GATHER_LOAD_COMMON1(FUN,OBJTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  memset (real_src, 0, (1 + NUM_SRC_ELEMS (STRIDE)) * sizeof (OBJTYPE));\
  memset (real_dst, 0, (1 + NUM_DST_ELEMS) * sizeof (OBJTYPE));\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    src[i * STRIDE] = i;\
  FUN##OBJTYPE##STRIDETYPE##STRIDE \
    (dst, src, NUM_DST_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    if (dst[i] != i)\
      abort ();\
}

#define TEST_GATHER_LOAD_COMMON2(FUN,OBJTYPE,STRIDETYPE,STRIDE)\
{\
  OBJTYPE real_src[1 + NUM_SRC_ELEMS (STRIDE)]\
    __attribute__((aligned (32)));\
  OBJTYPE real_dst[1 + NUM_DST_ELEMS]\
    __attribute__((aligned (32)));\
  memset (real_src, 0, (1 + NUM_SRC_ELEMS (STRIDE)) * sizeof (OBJTYPE));\
  memset (real_dst, 0, (1 + NUM_DST_ELEMS) * sizeof (OBJTYPE));\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst = &real_dst[1];\
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    src[i * STRIDE] = i;\
  FUN##OBJTYPE##STRIDETYPE \
    (dst, src, STRIDE, NUM_DST_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    if (dst[i] != i)\
      abort ();\
}

#define TEST_GATHER_LOAD1(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_GATHER_LOAD_COMMON1 (gather_load1, OBJTYPE, STRIDETYPE, STRIDE)

#define TEST_GATHER_LOAD2(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_GATHER_LOAD_COMMON2 (gather_load2, OBJTYPE, STRIDETYPE, STRIDE)

#define TEST_GATHER_LOAD3(OBJTYPE,STRIDETYPE)\
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
  memset (real_src, 0, (1 + NUM_SRC_ELEMS (5)) * sizeof (OBJTYPE));\
  memset (real_dst1, 0, (1 + NUM_DST_ELEMS) * sizeof (OBJTYPE));\
  memset (real_dst2, 0, (1 + NUM_DST_ELEMS) * sizeof (OBJTYPE));\
  memset (real_dst3, 0, (1 + NUM_DST_ELEMS) * sizeof (OBJTYPE));\
  memset (real_dst4, 0, (1 + NUM_DST_ELEMS) * sizeof (OBJTYPE));\
  memset (real_dst5, 0, (1 + NUM_DST_ELEMS) * sizeof (OBJTYPE));\
  OBJTYPE *src = &real_src[1];\
  OBJTYPE *dst1 = &real_dst1[1];\
  OBJTYPE *dst2 = &real_dst2[1];\
  OBJTYPE *dst3 = &real_dst3[1];\
  OBJTYPE *dst4 = &real_dst4[1];\
  OBJTYPE *dst5 = &real_dst5[1];\
  for (STRIDETYPE i = 0; i < NUM_SRC_ELEMS (5); i++)\
    src[i] = i;\
  gather_load3s5##OBJTYPE##STRIDETYPE \
    (dst1, dst2, dst3, dst4, dst5, src, NUM_DST_ELEMS); \
  for (STRIDETYPE i = 0; i < NUM_DST_ELEMS; i++)\
    {\
      STRIDETYPE base = i * 5;\
      if (dst1[i] != base)\
	abort ();\
      if (dst2[i] != (base + 1))\
	abort ();\
      if (dst3[i] != (base + 2))\
	abort ();\
      if (dst4[i] != (base + 3))\
	abort ();\
      if (dst5[i] != (base + 4))\
	abort ();\
    }\
}

#define TEST_GATHER_LOAD4(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_GATHER_LOAD_COMMON1 (gather_load4, OBJTYPE, STRIDETYPE, STRIDE)

#define TEST_GATHER_LOAD5(OBJTYPE,STRIDETYPE,STRIDE) \
  TEST_GATHER_LOAD_COMMON2 (gather_load5, OBJTYPE, STRIDETYPE, STRIDE)

int __attribute__ ((optimize (1)))
main ()
{
  TEST_GATHER_LOAD1 (double, long, 5);
  TEST_GATHER_LOAD1 (double, long, 8);
  TEST_GATHER_LOAD1 (double, long, 21);

  TEST_GATHER_LOAD1 (float, int, 5);
  TEST_GATHER_LOAD1 (float, int, 8);
  TEST_GATHER_LOAD1 (float, int, 21);

  TEST_GATHER_LOAD2 (double, long, 5);
  TEST_GATHER_LOAD2 (double, long, 8);
  TEST_GATHER_LOAD2 (double, long, 21);

  TEST_GATHER_LOAD2 (float, int, 5);
  TEST_GATHER_LOAD2 (float, int, 8);
  TEST_GATHER_LOAD2 (float, int, 21);

  TEST_GATHER_LOAD3 (double, long);
  TEST_GATHER_LOAD3 (float, int);

  TEST_GATHER_LOAD4 (double, long, 5);

  TEST_GATHER_LOAD5 (double, long, 5);
  TEST_GATHER_LOAD5 (float, int, 5);

  /* Widened forms.  */
  TEST_GATHER_LOAD1 (double, int, 5)
  TEST_GATHER_LOAD1 (double, int, 8)
  TEST_GATHER_LOAD1 (double, short, 5)
  TEST_GATHER_LOAD1 (double, short, 8)

  TEST_GATHER_LOAD1 (float, short, 5)
  TEST_GATHER_LOAD1 (float, short, 8)

  TEST_GATHER_LOAD2 (double, int, 5);
  TEST_GATHER_LOAD2 (double, int, 8);
  TEST_GATHER_LOAD2 (double, int, 21);

  TEST_GATHER_LOAD2 (float, short, 5);
  TEST_GATHER_LOAD2 (float, short, 8);
  TEST_GATHER_LOAD2 (float, short, 21);

  TEST_GATHER_LOAD4 (double, int, 5);
  TEST_GATHER_LOAD4 (float, short, 5);

  TEST_GATHER_LOAD5 (double, int, 5);

  return 0;
}

