/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O -march=armv8-a+sve" } */

#include "sve_vec_perm_const_single_1.c"
extern void abort (void);

#define TEST_VEC_PERM(TYPE, EXPECTED_RESULT, VALUES1, VALUES2)		\
{									\
  TYPE expected_result = EXPECTED_RESULT;				\
  TYPE values1 = VALUES1;						\
  TYPE values2 = VALUES2;						\
  TYPE dest;								\
  dest = vec_perm_##TYPE (values1, values2);				\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    abort ();								\
}

int main (void)
{
  TEST_VEC_PERM (v4di,
		 ((v4di) { 4, 7, 6, 5 }),
		 ((v4di) { 4, 5, 6, 7 }),
		 ((v4di) { 12, 24, 36, 48 }));
  TEST_VEC_PERM (v8si,
		 ((v8si) { 6, 10, 4, 3, 5, 7, 7, 5 }),
		 ((v8si) { 3, 4, 5, 6, 7, 8, 9, 10 }),
		 ((v8si) { 33, 34, 35, 36, 37, 38, 39, 40 }));
  TEST_VEC_PERM (v16hi,
		 ((v16hi) { 11, 10, 8, 7, 14, 15, 16, 3,
			    4, 4, 11, 12, 6, 17, 18, 4 }),
		 ((v16hi) { 3, 4, 5, 6, 7, 8, 9, 10,
			    11, 12, 13, 14, 15, 16, 17, 18 }),
		 ((v16hi) { 33, 34, 35, 36, 37, 38, 39, 40,
			    41, 42, 43, 44, 45, 46, 47, 48 }));
  TEST_VEC_PERM (v32qi,
		 ((v32qi) { 5, 5, 7, 6, 4, 4, 7, 4,
			    6, 7, 6, 5, 4, 7, 6, 7,
			    4, 5, 7, 7, 4, 7, 4, 5,
			    6, 7, 6, 7, 6, 4, 6, 5 }),
		 ((v32qi) { 4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7 }),
		 ((v32qi) { 12, 24, 36, 48, 12, 24, 36, 48,
			    12, 24, 36, 48, 12, 24, 36, 48,
			    12, 24, 36, 48, 12, 24, 36, 48,
			    12, 24, 36, 48, 12, 24, 36, 48 }));
  TEST_VEC_PERM (v4df,
		 ((v4df) { 7.5, 7.5, 5.5, 5.5 }),
		 ((v4df) { 4.5, 5.5, 6.5, 7.5 }),
		 ((v4df) { 12.5, 24.5, 36.5, 48.5 }));
  TEST_VEC_PERM (v8sf,
		 ((v8sf) { 7.5, 8.5, 9.5, 3.5, 5.5, 10.5, 7.5, 5.5 }),
		 ((v8sf) { 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5 }),
		 ((v8sf) { 33.5, 34.5, 35.5, 36.5,
			   37.5, 38.5, 39.5, 40.5 }));
  return 0;
}
