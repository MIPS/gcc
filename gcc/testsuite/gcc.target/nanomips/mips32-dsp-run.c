/* Test MIPS32 DSP instructions */
/* { dg-do run } */
/* { dg-options "-march=32r6 -mdspr3 (REQUIRES_STDLIB)" } */

#include <stdlib.h>
#include <stdio.h>

typedef signed char v4i8 __attribute__ ((vector_size(4)));
typedef short v2q15 __attribute__ ((vector_size(4)));

typedef int q31;
typedef int i32;
typedef unsigned int ui32;
typedef long long a64;

void test_MIPS_DSP (void);

char array[100];
int little_endian;

int main ()
{
  int i;

  union { long long ll; int i[2]; } endianness_test;
  endianness_test.ll = 1;
  little_endian = endianness_test.i[0];

  for (i = 0; i < 100; i++)
    array[i] = i;

  test_MIPS_DSP ();

  exit (0);
}

v2q15 add_v2q15 (v2q15 a, v2q15 b)
{
  return __builtin_mips_addq_ph (a, b);
}

v4i8 add_v4i8 (v4i8 a, v4i8 b)
{
  return __builtin_mips_addu_qb (a, b);
}

v2q15 sub_v2q15 (v2q15 a, v2q15 b)
{
  return __builtin_mips_subq_ph (a, b);
}

v4i8 sub_v4i8 (v4i8 a, v4i8 b)
{
  return __builtin_mips_subu_qb (a, b);
}

void test_MIPS_DSP ()
{
  v4i8 v4i8_a,v4i8_b,v4i8_c,v4i8_r,v4i8_s;
  v2q15 v2q15_a,v2q15_b,v2q15_c,v2q15_r,v2q15_s;
  q31 q31_a,q31_b,q31_c,q31_r,q31_s;
  /* To protect the multiplication-related tests from being optimized
     at compile time.  */
  volatile i32 i32_a,i32_b,i32_c,i32_r,i32_s;
  volatile ui32 ui32_a,ui32_b,ui32_c;
  a64 a64_a,a64_b,a64_c,a64_r,a64_s;

  void *ptr_a;
  int r,s;
  long long lr,ls;

  v2q15_a = (v2q15) {0x1234, 0x5678};
  v2q15_b = (v2q15) {0x1234, 0x7856};
  i32_s = 0x03000000;
  __builtin_mips_cmp_le_ph (v2q15_a, v2q15_b);
  i32_r = __builtin_mips_rddsp (16);
  if (i32_r != i32_s)
    abort ();

}

