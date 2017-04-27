/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math --save-temps" } */

#define TEST_LOOP(NAME, DATA_TYPE, INDEX_TYPE)				\
  DATA_TYPE __attribute__ ((noinline))					\
  NAME (char *data, INDEX_TYPE *indices, signed char n)			\
  {									\
    DATA_TYPE sum = 0;							\
    for (signed char i = 0; i < n; ++i)					\
      {									\
        INDEX_TYPE index = indices[i];					\
        sum += (index & 16 ? *(DATA_TYPE *) (data + index) : 1);	\
      }									\
    return sum;								\
  }

TEST_LOOP (f_s32, int, unsigned int)
TEST_LOOP (f_u32, unsigned int, unsigned int)
TEST_LOOP (f_f32, float, unsigned int)

TEST_LOOP (f_s64_s64, long, long)
TEST_LOOP (f_s64_u64, long, unsigned long)
TEST_LOOP (f_u64_s64, unsigned long, long)
TEST_LOOP (f_u64_u64, unsigned long, unsigned long)
TEST_LOOP (f_f64_s64, double, long)
TEST_LOOP (f_f64_u64, double, unsigned long)

/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, z[0-9]+\.s, uxtw\]} 3 } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z, \[x[0-9]+, z[0-9]+\.d\]} 6 } } */
