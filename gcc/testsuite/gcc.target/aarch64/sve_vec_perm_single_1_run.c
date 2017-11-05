/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O -march=armv8-a+sve" } */
/* { dg-options "-O -march=armv8-a+sve -msve-vector-bits=256" { target aarch64_sve256_hw } } */

#include "sve_vec_perm_single_1.c"
extern void abort (void);

#define TEST_VEC_PERM(TYPE, MASK_TYPE, EXPECTED_RESULT, VALUES, MASK)	\
{									\
  TYPE expected_result = EXPECTED_RESULT;				\
  TYPE values = VALUES;							\
  MASK_TYPE mask = MASK;						\
  TYPE dest;								\
  dest = vec_perm_##TYPE (values, mask);				\
  if (__builtin_memcmp (&dest, &expected_result, sizeof (TYPE)) != 0)	\
    __builtin_abort ();							\
}

int main (void)
{
  TEST_VEC_PERM (v4di, v4di,
		 ((v4di) { 5, 6, 7, 5 }),
		 ((v4di) { 4, 5, 6, 7 }),
		 ((v4di) { 1, 6, 3, 5 }));
  TEST_VEC_PERM (v8si, v8si,
		 ((v8si) { 4, 8, 10, 10, 9, 8, 7, 5 }),
		 ((v8si) { 3, 4, 5, 6, 7, 8, 9, 10 }),
		 ((v8si) { 9, 13, 15, 7, 6, 5, 4, 10 }));
  TEST_VEC_PERM (v16hi, v16hi,
		 ((v16hi) { 12, 16, 18, 10, 12, 13, 14, 4,
			    7, 18, 3, 5, 9, 8, 7, 13 }),
		 ((v16hi) { 3, 4, 5, 6, 7, 8, 9, 10,
			    11, 12, 13, 14, 15, 16, 17, 18 }),
		 ((v16hi) { 9, 13, 15, 7, 25, 26, 27, 17,
			    4, 31, 0, 18, 6, 5, 4, 10 }));
  TEST_VEC_PERM (v32qi, v32qi,
		 ((v32qi) { 5, 6, 7, 4, 5, 6, 4, 5,
			    6, 7, 4, 5, 6, 7, 4, 5,
			    5, 6, 7, 4, 5, 6, 4, 5,
			    6, 7, 4, 5, 6, 7, 4, 5 }),
		 ((v32qi) { 4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7,
			    4, 5, 6, 7, 4, 5, 6, 7 }),
		 ((v32qi) { 5, 6, 7, 8, 9, 10, 28, 29,
			    30, 31, 32, 33, 54, 55, 56, 61,
			    5, 6, 7, 8, 9, 10, 28, 29,
			    30, 31, 32, 33, 54, 55, 56, 61 }));
  TEST_VEC_PERM (v4df, v4di,
		 ((v4df) { 5.1, 6.1, 7.1, 5.1 }),
		 ((v4df) { 4.1, 5.1, 6.1, 7.1 }),
		 ((v4di) { 1, 6, 3, 5 }));
  TEST_VEC_PERM (v8sf, v8si,
		 ((v8sf) { 4.2, 8.2, 10.2, 10.2, 9.2, 8.2, 7.2, 5.2 }),
		 ((v8sf) { 3.2, 4.2, 5.2, 6.2, 7.2, 8.2, 9.2, 10.2 }),
		 ((v8si) { 9, 13, 15, 7, 6, 5, 4, 10 }));
  TEST_VEC_PERM (v16hf, v16hi,
		 ((v16hf) { 12.0, 16.0, 18.0, 10.0, 12.0, 13.0, 14.0, 4.0,
			    7.0, 18.0, 3.0, 5.0, 9.0, 8.0, 7.0, 13.0 }),
		 ((v16hf) { 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
			    11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 }),
		 ((v16hi) { 9, 13, 15, 7, 25, 26, 27, 17,
			    4, 31, 0, 18, 6, 5, 4, 10 }));
  return 0;
}
