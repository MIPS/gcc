/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O -march=armv8-a+sve" } */

#include "sve_uzp1_1.c"
extern void abort (void);

#define TEST_UZP1(TYPE, EXPECTED_RESULT, VALUES1, VALUES2)		\
{									\
  TYPE expected_result = EXPECTED_RESULT;				\
  TYPE values1 = VALUES1;						\
  TYPE values2 = VALUES2;						\
  TYPE dest;								\
  dest = uzp1_##TYPE (values1, values2);				\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    abort ();								\
}

int main (void)
{
  TEST_UZP1 (v4di,
	     ((v4di) { 4, 6, 12, 36 }),
	     ((v4di) { 4, 5, 6, 7 }),
	     ((v4di) { 12, 24, 36, 48 }));
  TEST_UZP1 (v8si,
	     ((v8si) { 3, 5, 7, 9, 33, 35, 37, 39 }),
	     ((v8si) { 3, 4, 5, 6, 7, 8, 9, 10 }),
	     ((v8si) { 33, 34, 35, 36, 37, 38, 39, 40 }));
  TEST_UZP1 (v16hi,
	     ((v16hi) { 3, 5, 7, 9, 11, 13, 15, 17,
			33, 35, 37, 39, 41, 43, 45, 47 }),
	     ((v16hi) { 3, 4, 5, 6, 7, 8, 9, 10,
			11, 12, 13, 14, 15, 16, 17, 18 }),
	     ((v16hi) { 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48 }));
  TEST_UZP1 (v32qi,
	     ((v32qi) { 4, 6, 4, 6, 4, 6, 4, 6,
			4, 6, 4, 6, 4, 6, 4, 6,
			12, 36, 12, 36, 12, 36, 12, 36,
			12, 36, 12, 36, 12, 36, 12, 36 }),
	     ((v32qi) { 4, 5, 6, 7, 4, 5, 6, 7,
			4, 5, 6, 7, 4, 5, 6, 7,
			4, 5, 6, 7, 4, 5, 6, 7,
			4, 5, 6, 7, 4, 5, 6, 7 }),
	     ((v32qi) { 12, 24, 36, 48, 12, 24, 36, 48,
			12, 24, 36, 48, 12, 24, 36, 48,
			12, 24, 36, 48, 12, 24, 36, 48,
			12, 24, 36, 48, 12, 24, 36, 48 }));
  TEST_UZP1 (v4df,
	     ((v4df) { 4.0, 6.0, 12.0, 36.0 }),
	     ((v4df) { 4.0, 5.0, 6.0, 7.0 }),
	     ((v4df) { 12.0, 24.0, 36.0, 48.0 }));
  TEST_UZP1 (v8sf,
	     ((v8sf) { 3.0, 5.0, 7.0, 9.0, 33.0, 35.0, 37.0, 39.0 }),
	     ((v8sf) { 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 }),
	     ((v8sf) { 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0, 40.0 }));
  return 0;
}
