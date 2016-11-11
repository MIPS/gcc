/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sve_unpack_unsigned_1.c"

#define ARRAY_SIZE 85

#define RUN_AND_CHECK_LOOP(TYPED, TYPES, VALUED, VALUES)		\
{									\
  int value = 0;							\
  TYPED arrayd[ARRAY_SIZE];						\
  TYPES arrays[ARRAY_SIZE];						\
  memset (arrayd, 67, ARRAY_SIZE * sizeof (TYPED));			\
  memset (arrays, VALUES, ARRAY_SIZE * sizeof (TYPES));			\
  unpack_##TYPED##_##TYPES##_signed (arrayd, arrays, ARRAY_SIZE);	\
  for (int i = 0; i < ARRAY_SIZE; i++)					\
    if (arrayd[i] != VALUED)						\
      {									\
	fprintf (stderr,"%d: %d != %d\n", i, arrayd[i], VALUED);	\
	exit (1);							\
      }									\
  memset (arrayd, 74, ARRAY_SIZE * sizeof (TYPED));			\
  unpack_##TYPED##_##TYPES##_unsigned (arrayd, arrays, ARRAY_SIZE);	\
  for (int i = 0; i < ARRAY_SIZE; i++)					\
    if (arrayd[i] != VALUED)						\
      {									\
	fprintf (stderr,"%d: %d != %d\n", i, arrayd[i], VALUED);	\
	exit (1);							\
      }									\
}

int main (void)
{
  int total = 5;
  RUN_AND_CHECK_LOOP (short, char, total + 1, total);
  total = (total << 8) + 5;
  RUN_AND_CHECK_LOOP (int, short, total + 1, total);
  total = (total << 8) + 5;
  total = (total << 8) + 5;
  RUN_AND_CHECK_LOOP (long, int, total + 1, total);
  return 0;
}
