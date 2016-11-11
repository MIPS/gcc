/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O -march=armv8-a+sve" } */

#include "sve_vec_perm_1.c"
extern void abort (void);

#define TEST_VEC_PERM(TYPE, MASK_TYPE, EXPECTED_RESULT,			\
		      VALUES1, VALUES2, MASK)				\
{									\
  TYPE expected_result = EXPECTED_RESULT;				\
  TYPE values1 = VALUES1;						\
  TYPE values2 = VALUES2;						\
  MASK_TYPE mask = MASK;						\
  TYPE dest;								\
  dest = vec_perm_##TYPE (values1, values2, mask);			\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    abort ();								\
}

int main (void)
{
  TEST_VEC_PERM (v4di, v4di,
		 ((v4di) { 5, 36, 7, 48 }),
		 ((v4di) { 4, 5, 6, 7 }),
		 ((v4di) { 12, 24, 36, 48 }),
		 ((v4di) { 1, 6, 3, 7 }));
  TEST_VEC_PERM (v8si, v8si,
		 ((v8si) { 34, 38, 40, 10, 9, 8, 7, 35 }),
		 ((v8si) { 3, 4, 5, 6, 7, 8, 9, 10 }),
		 ((v8si) { 33, 34, 35, 36, 37, 38, 39, 40 }),
		 ((v8si) { 9, 13, 15, 7, 6, 5, 4, 10 }));
  TEST_VEC_PERM (v16hi, v16hi,
		 ((v16hi) { 12, 16, 18, 10, 42, 43, 44, 34,
			    7, 48, 3, 35, 9, 8, 7, 13 }),
		 ((v16hi) { 3, 4, 5, 6, 7, 8, 9, 10,
			    11, 12, 13, 14, 15, 16, 17, 18 }),
		 ((v16hi) { 33, 34, 35, 36, 37, 38, 39, 40,
			    41, 42, 43, 44, 45, 46, 47, 48 }),
		 ((v16hi) { 9, 13, 15, 7, 25, 26, 27, 17,
			    4, 31, 0, 18, 6, 5, 4, 10 }));
  TEST_VEC_PERM (v32qi, v32qi,
		 ((v32qi) { 5, 6, 7, 4, 5, 6, 4, 5,
			    6, 7, 12, 24, 36, 48, 12, 24,
			    5, 6, 7, 4, 5, 6, 4, 5,
			    6, 7, 12, 24, 36, 48, 12, 24 }),
		 ((v32qi) { 4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7 }),
		 ((v32qi) { 12, 24, 36, 48, 12, 24, 36, 48,
			    12, 24, 36, 48, 12, 24, 36, 48,
			    12, 24, 36, 48, 12, 24, 36, 48,
			    12, 24, 36, 48, 12, 24, 36, 48 }),
		 ((v32qi) { 5, 6, 7, 8, 9, 10, 28, 29,
			    30, 31, 32, 33, 54, 55, 56, 61,
			    5, 6, 7, 8, 9, 10, 28, 29,
			    30, 31, 32, 33, 54, 55, 56, 61 }));
  TEST_VEC_PERM (v4df, v4di,
		 ((v4df) { 5.1, 36.1, 7.1, 48.1 }),
		 ((v4df) { 4.1, 5.1, 6.1, 7.1 }),
		 ((v4df) { 12.1, 24.1, 36.1, 48.1 }),
		 ((v4di) { 1, 6, 3, 7 }));
  TEST_VEC_PERM (v8sf, v8si,
		 ((v8sf) { 34.2, 38.2, 40.2, 10.2, 9.2, 8.2, 7.2, 35.2 }),
		 ((v8sf) { 3.2, 4.2, 5.2, 6.2, 7.2, 8.2, 9.2, 10.2 }),
		 ((v8sf) { 33.2, 34.2, 35.2, 36.2,
			   37.2, 38.2, 39.2, 40.2 }),
		 ((v8si) { 9, 13, 15, 7, 6, 5, 4, 10 }));
  return 0;
}
