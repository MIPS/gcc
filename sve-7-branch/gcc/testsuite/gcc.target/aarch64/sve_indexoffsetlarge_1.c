/* { dg-do assemble } */
/* { dg-options "-std=c99 -ftree-vectorize -O2 -fno-inline -march=armv8-a+sve --save-temps" } */

//Test sizes that are too big for an index register
#define SIZE 4294967297

#define INDEX_OFFSET_TEST(SIGNED, TYPE)\
void set_##SIGNED##TYPE (SIGNED TYPE *out, SIGNED TYPE *in)\
{\
  unsigned long i;\
  for (i = 0; i < SIZE; i++)\
  {\
    out[i] = in[i];\
  }\
}

INDEX_OFFSET_TEST (signed, int)
INDEX_OFFSET_TEST (unsigned, int)
INDEX_OFFSET_TEST (signed, short)
INDEX_OFFSET_TEST (unsigned, short)
INDEX_OFFSET_TEST (signed, char)
INDEX_OFFSET_TEST (unsigned, char)

/* { dg-final { scan-assembler-not "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, w\[0-9\]+, .xtw 3\\\]" } } */
/* { dg-final { scan-assembler-not "st1d\\tz\[0-9\]+.d, p\[0-9\]+, \\\[x\[0-9\]+, w\[0-9\]+, .xtw 3\\\]" } } */
/* { dg-final { scan-assembler-not "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, w\[0-9\]+, .xtw 2\\\]" } } */
/* { dg-final { scan-assembler-not "st1w\\tz\[0-9\]+.s, p\[0-9\]+, \\\[x\[0-9\]+, w\[0-9\]+, .xtw 2\\\]" } } */
/* { dg-final { scan-assembler-not "ld1h\\tz\[0-9\]+.h, p\[0-9\]+/z, \\\[x\[0-9\]+, w\[0-9\]+, .xtw 1\\\]" } } */
/* { dg-final { scan-assembler-not "st1h\\tz\[0-9\]+.h, p\[0-9\]+, \\\[x\[0-9\]+, w\[0-9\]+, .xtw 1\\\]" } } */
/* { dg-final { scan-assembler-not "ld1b\\tz\[0-9\]+.b, p\[0-9\]+/z, \\\[x\[0-9\]+, w\[0-9\]+, .xtw\\\]" } } */
/* { dg-final { scan-assembler-not "st1b\\tz\[0-9\]+.b, p\[0-9\]+, \\\[x\[0-9\]+, w\[0-9\]+, .xtw\\\]" } } */
