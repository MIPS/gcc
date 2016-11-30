/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -msve-vector-bits=256" } */

#include "sve_speculative_15.c"

extern void abort (void);

#ifndef FILL_DATA
#define FILL_DATA 0
#endif

#ifndef EXIT_CONDITION
#define EXIT_CONDITION 5
#endif

#ifndef LOOP_COUNTS
#define LOOP_COUNTS {37,45,55,17,39,43}
#endif
int loop_counts[] = LOOP_COUNTS;

/* Fill the arrays with the exit conditions.
   Then refill at the correct strided accesses with fill data up to the end of
   the loop count.  */

#define TEST_SPEC_LOOP_FUNC(DATATYPE, ARGTYPE)\
void test_spec_loop_##DATATYPE##_##ARGTYPE (ARGTYPE num_elements)\
{\
  int i;\
  for (i=0; i<MAX_ARRAY_SIZE; i++)\
    {\
      a##DATATYPE[i] = EXIT_CONDITION;\
    }\
  for (i=0; (i<num_elements-1)*STRIDE_LEVEL; i++)\
    {\
      a##DATATYPE[i*STRIDE_LEVEL] = FILL_DATA;\
      a##DATATYPE[i*STRIDE_LEVEL+1] = FILL_DATA;\
    }\
  ARGTYPE ret = spec_loop_##DATATYPE##_##ARGTYPE (EXIT_CONDITION);\
  if (ret != num_elements - 1)\
    abort ();\
}

TEST_SPEC_LOOP_FUNC (int8_t, int8_t)
TEST_SPEC_LOOP_FUNC (int16_t, int16_t)
TEST_SPEC_LOOP_FUNC (int32_t, int32_t)
TEST_SPEC_LOOP_FUNC (int64_t, int64_t)
TEST_SPEC_LOOP_FUNC (float, int32_t)
TEST_SPEC_LOOP_FUNC (double, int64_t)

int main (void)
{
  test_spec_loop_int8_t_int8_t (loop_counts[0]);
  test_spec_loop_int16_t_int16_t (loop_counts[1]);
  test_spec_loop_int32_t_int32_t (loop_counts[2]);
  test_spec_loop_int64_t_int64_t (loop_counts[3]);
  test_spec_loop_float_int32_t (loop_counts[4]);
  test_spec_loop_double_int64_t (loop_counts[5]);
  return 0;
}

