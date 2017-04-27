/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-inline -march=armv8-a+sve -msve-vector-bits=256" } */

#include "sve_speculative_5.c"

#define _GNU_SOURCE
#include <sys/mman.h>
extern void abort (void);
extern void *mremap (void *old_address, size_t old_size,
		     size_t new_size, int flags, ... /* void *new_address */);

#ifndef FILL_DATA
#define FILL_DATA 0
#endif

#ifndef LOOP_COUNTS
#define LOOP_COUNTS {22,20,13,17,29,19}
#endif
int loop_counts[] = LOOP_COUNTS;

/* Program will fault if memory beyond the boundaries of BUF is accessed.  */

#define SPACE_SIZE 4096*sizeof(int)

/* Enable to confirm program segfaults when accessing outside of BUF.  */
#ifdef CHECK_SEGFAULT
#define ADDITIONAL 1
#else
#define ADDITIONAL 0
#endif

/* BUF is an array of NUM_ELEMENTS size.
   BUF_PRE points to 4 elements before BUF.
   Before calling SPEC_LOOP, set the last element of BUF and the
   four elements of BUF_PRE to the exit condition.
   Fill the rest of BUF to the fill data.  */

#define TEST_SPEC_LOOP_FUNC(ARGTYPE)\
void test_spec_loop_##ARGTYPE (void *bufend, ARGTYPE num_elements)\
{\
  int i;\
  ARGTYPE* buf = ((ARGTYPE*)bufend) - num_elements;\
  ARGTYPE* buf_pre = ((ARGTYPE*)bufend) - num_elements - 4;\
  for (i=0; i<num_elements-1; i++)\
    buf[i] = FILL_DATA;\
  buf[num_elements - 1 + ADDITIONAL] = EXIT_CONDITION;\
  for (i=0; i<4; i++)\
    buf_pre[i] = EXIT_CONDITION;\
  ARGTYPE ret = spec_loop_##ARGTYPE (buf);\
  if (ret != num_elements)\
    abort ();\
}

#define TEST_SPEC_FP_LOOP_FUNC(FPTYPE, ARGTYPE)\
void test_spec_loop_##ARGTYPE##FPTYPE (void *bufend, ARGTYPE num_elements)\
{\
  int i;\
  FPTYPE* buf = ((FPTYPE*)bufend) - num_elements;\
  FPTYPE* buf_pre = ((FPTYPE*)bufend) - num_elements - 4;\
  for (i=0; i<num_elements-1; i++)\
    buf[i] = FILL_DATA;\
  buf[num_elements - 1 + ADDITIONAL] = EXIT_CONDITION;\
  for (i=0; i<4; i++)\
    buf_pre[i] = EXIT_CONDITION;\
  ARGTYPE ret = spec_loop_##ARGTYPE##FPTYPE (buf);\
  if (ret != num_elements)\
    abort ();\
}

TEST_SPEC_LOOP_FUNC (int8_t)
TEST_SPEC_LOOP_FUNC (int16_t)
TEST_SPEC_LOOP_FUNC (int32_t)
TEST_SPEC_LOOP_FUNC (int64_t)
TEST_SPEC_FP_LOOP_FUNC (float, int32_t)
TEST_SPEC_FP_LOOP_FUNC (double, int64_t)

int main (void)
{
  /* Map in two pages worth of space.  Then reduce it down to a single page.
     This will result in the second page of data being unmapped - ie it
     will cause a segfault if accessed.  */

  void *space = mmap (0, SPACE_SIZE * 2, PROT_READ|PROT_WRITE,
		      MAP_ANON|MAP_PRIVATE, -1, 0);
  if (space == (void*)-1)
    abort ();

  void *space_new = mremap (space, SPACE_SIZE * 2, SPACE_SIZE, 0);
  if (space != space_new)
    abort ();

  /* set END to the start of the second (unmapped) page.  */
  char *end = space + SPACE_SIZE;

  test_spec_loop_int8_t (end, loop_counts[0]);
  test_spec_loop_int16_t (end, loop_counts[1]);
  test_spec_loop_int32_t (end, loop_counts[2]);
  test_spec_loop_int64_t (end, loop_counts[3]);
  test_spec_loop_int32_tfloat (end, loop_counts[4]);
  test_spec_loop_int64_tdouble (end, loop_counts[5]);

  return 0;
}
