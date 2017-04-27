/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sve_pack_fcvt_signed_1.c"

#define ARRAY_SIZE 107

#define VAL1 ((i * 345.434) - (19 * 345.434))

int __attribute__ ((optimize (1)))
main (void)
{
  static signed int array_dest[ARRAY_SIZE];
  double array_source[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    array_source[i] = VAL1;

  pack_int_double_plus_3 (array_dest, array_source, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_dest[i] != (int) VAL1 + 3)
      {
	fprintf (stderr,"%d: %d != %d\n", i, array_dest[i], (int) VAL1 + 3);
	exit (1);
      }

  return 0;
}
