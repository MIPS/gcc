/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sve_pack_float_1.c"

#define ARRAY_SIZE 107

#define VAL1 ((i * 886.556) - (43 * 886.556))

int __attribute__ ((optimize (1)))
main (void)
{
  float array_dest[ARRAY_SIZE];
  double array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    array_source[i] = VAL1;

  pack_float_plus_1point1 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (float) (VAL1 + 1.1))
      {
	fprintf (stderr, "%d: %f != %f\n", i, array_dest[i],
		 (float) (VAL1 + 1.1));
	exit (1);
      }

  return 0;
}
