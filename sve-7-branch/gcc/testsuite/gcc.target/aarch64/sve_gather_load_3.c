/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math --save-temps" } */

#define TEST_LOOP(NAME, DATA_TYPE, INDEX_TYPE)		\
  DATA_TYPE __attribute__ ((noinline))			\
  NAME (char *data, INDEX_TYPE *indices, signed char n)	\
  {							\
    DATA_TYPE sum = 0;					\
    for (signed char i = 0; i < n; ++i)			\
      sum += *(DATA_TYPE *) (data + indices[i]);	\
    return sum;						\
  }

#define TEST32(NAME, DATA_TYPE)				\
  TEST_LOOP (NAME ## _u8, DATA_TYPE, unsigned char)	\
  TEST_LOOP (NAME ## _u16, DATA_TYPE, unsigned short)	\
  TEST_LOOP (NAME ## _u32, DATA_TYPE, unsigned int)     \
  TEST_LOOP (NAME ## _s8, DATA_TYPE, signed char)	\
  TEST_LOOP (NAME ## _s16, DATA_TYPE, signed short)	\
  TEST_LOOP (NAME ## _s32, DATA_TYPE, signed int)

#define TEST64(NAME, DATA_TYPE)				\
  TEST_LOOP (NAME ## _s8, DATA_TYPE, signed char)	\
  TEST_LOOP (NAME ## _u8, DATA_TYPE, unsigned char)	\
  TEST_LOOP (NAME ## _s16, DATA_TYPE, short)		\
  TEST_LOOP (NAME ## _u16, DATA_TYPE, unsigned short)	\
  TEST_LOOP (NAME ## _s32, DATA_TYPE, int)		\
  TEST_LOOP (NAME ## _u32, DATA_TYPE, unsigned int)	\
  TEST_LOOP (NAME ## _s64, DATA_TYPE, long)		\
  TEST_LOOP (NAME ## _u64, DATA_TYPE, unsigned long)

TEST32 (f_s32, int)
TEST32 (f_u32, unsigned int)
TEST32 (f_f32, float)

TEST64 (f_s64, long)
TEST64 (f_u64, unsigned long)
TEST64 (f_f64, double)

/* (4 + 2 + 1) * 3 */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, z[0-9]+\.s, sxtw\]} 21 } } */
/* (4 + 2 + 1) * 3 */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, z[0-9]+\.s, uxtw\]} 21 } } */
/* (8 + 8 + 4 + 4 + 2 + 2 + 1 + 1) * 3 */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z, \[x[0-9]+, z[0-9]+\.d\]} 90 } } */
