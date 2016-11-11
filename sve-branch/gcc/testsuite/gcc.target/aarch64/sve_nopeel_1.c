/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -msve-vector-bits=256" } */

#define TEST(NAME, TYPE, ITYPE)			\
 void						\
 NAME##1 (TYPE *x, ITYPE n)			\
 {						\
   for (ITYPE i = 0; i < n; ++i)		\
     x[i] += 1;					\
 }						\
 TYPE NAME##_array[1024];			\
 void						\
 NAME##2 (void)					\
 {						\
   for (ITYPE i = 1; i < 200; ++i)		\
     NAME##_array[i] += 1;			\
 }

TEST (sc, signed char, unsigned char)
TEST (uc, unsigned char, unsigned char)
TEST (ss, signed short, unsigned short)
TEST (us, unsigned short, signed short)
TEST (si, signed int, signed int)
TEST (ui, unsigned int, unsigned int)
TEST (sl, signed long, unsigned long)
TEST (ul, unsigned long, signed long)
TEST (f, float, int)
TEST (d, double, long)

/* No scalar memory accesses.  */
/* { dg-final { scan-assembler-not {[wx][0-9]*, \[} } } */
/* 2 for each NAME##1 test, one in the header and one in the main loop
   and 1 for each NAME##2 test, in the main loop only.  */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b,} 6 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h,} 6 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s,} 9 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d,} 9 } } */
