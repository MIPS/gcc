/* { dg-do compile } */
/* { dg-options "-std=c99 -ftree-vectorize -O2 -fno-inline -march=armv8-a+sve -msve-vector-bits=256" } */

#define SIZE 15*8+3

#define INDEX_OFFSET_TEST_1(SIGNED, TYPE, ITERTYPE) \
void set_##SIGNED##_##TYPE##_##ITERTYPE (SIGNED TYPE *__restrict out, \
					 SIGNED TYPE *__restrict in) \
{ \
  SIGNED ITERTYPE i; \
  for (i = 0; i < SIZE; i++) \
  { \
    out[i] = in[i]; \
  } \
} \
void set_##SIGNED##_##TYPE##_##ITERTYPE##_var (SIGNED TYPE *__restrict out, \
					       SIGNED TYPE *__restrict in, \
					       SIGNED ITERTYPE n) \
{\
  SIGNED ITERTYPE i;\
  for (i = 0; i < n; i++)\
  {\
    out[i] = in[i];\
  }\
}

#define INDEX_OFFSET_TEST(SIGNED, TYPE)\
  INDEX_OFFSET_TEST_1 (SIGNED, TYPE, char) \
  INDEX_OFFSET_TEST_1 (SIGNED, TYPE, short) \
  INDEX_OFFSET_TEST_1 (SIGNED, TYPE, int) \
  INDEX_OFFSET_TEST_1 (SIGNED, TYPE, long)

INDEX_OFFSET_TEST (signed, long)
INDEX_OFFSET_TEST (unsigned, long)
INDEX_OFFSET_TEST (signed, int)
INDEX_OFFSET_TEST (unsigned, int)
INDEX_OFFSET_TEST (signed, short)
INDEX_OFFSET_TEST (unsigned, short)
INDEX_OFFSET_TEST (signed, char)
INDEX_OFFSET_TEST (unsigned, char)

/* { dg-final { scan-assembler-times "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, x\[0-9\]+, lsl 3\\\]" 16 } } */
/* { dg-final { scan-assembler-times "st1d\\tz\[0-9\]+.d, p\[0-9\]+, \\\[x\[0-9\]+, x\[0-9\]+, lsl 3\\\]" 16 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, x\[0-9\]+, lsl 2\\\]" 16 } } */
/* { dg-final { scan-assembler-times "st1w\\tz\[0-9\]+.s, p\[0-9\]+, \\\[x\[0-9\]+, x\[0-9\]+, lsl 2\\\]" 16 } } */
/* { dg-final { scan-assembler-times "ld1h\\tz\[0-9\]+.h, p\[0-9\]+/z, \\\[x\[0-9\]+, x\[0-9\]+, lsl 1\\\]" 16 } } */
/* { dg-final { scan-assembler-times "st1h\\tz\[0-9\]+.h, p\[0-9\]+, \\\[x\[0-9\]+, x\[0-9\]+, lsl 1\\\]" 16 } } */
/* { dg-final { scan-assembler-times "ld1b\\tz\[0-9\]+.b, p\[0-9\]+/z, \\\[x\[0-9\]+, x\[0-9\]+\\\]" 16 } } */
/* { dg-final { scan-assembler-times "st1b\\tz\[0-9\]+.b, p\[0-9\]+, \\\[x\[0-9\]+, x\[0-9\]+\\\]" 16 } } */
