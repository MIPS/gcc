/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sve_fcvtz_signed_1.c"

#define ARRAY_SIZE 81

#define VAL1 ((i * 237.86) - (29 * 237.86))
#define VAL2 ((double) ((i * 0xf8dfef2f) - (11 * 0xf8dfef2f)))

int __attribute__ ((optimize (1)))
main (void)
{
  static signed int array_desti[ARRAY_SIZE];
  static signed long array_destl[ARRAY_SIZE];

  float array_source_f[ARRAY_SIZE];
  double array_source_d[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source_f[i] = VAL1;
      array_source_d[i] = VAL2;
    }

  vfcvtz_32 (array_desti, array_source_f, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_desti[i] != (int) VAL1)
      {
	fprintf (stderr,"%d: %d != %d\n", i, array_desti[i], (int) VAL1);
	exit (1);
      }

  vfcvtz_64 (array_destl, array_source_d, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_destl[i] != (long) VAL2)
      {
	fprintf (stderr,"%d: %ld != %ld\n", i, array_destl[i], (long) VAL2);
	exit (1);
      }

  return 0;
}
