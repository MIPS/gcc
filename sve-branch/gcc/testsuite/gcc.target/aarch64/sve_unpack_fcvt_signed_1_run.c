/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sve_unpack_fcvt_signed_1.c"

#define ARRAY_SIZE 89

#define VAL1 ((i * 88654) - (33 * 88654))

int __attribute__ ((optimize (1)))
main (void)
{
  double array_dest[ARRAY_SIZE];
  signed int array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    array_source[i] = VAL1;

  unpack_double_int_plus8 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (float) (VAL1 + 8))
      {
	fprintf (stderr,"%d: %f != %f\n", i, array_dest[i],
		 (float) (VAL1 + 8));
	exit (1);
      }

  return 0;
}
