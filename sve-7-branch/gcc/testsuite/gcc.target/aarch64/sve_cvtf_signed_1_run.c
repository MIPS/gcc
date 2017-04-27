/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "sve_cvtf_signed_1.c"

#define ARRAY_SIZE 47

#define VAL1 ((i * 3) - (15 * 3))
#define VAL2 ((i * 0xffdfffef) - (11 * 0xffdfffef))

int __attribute__ ((optimize (1)))
main (void)
{
  static float array_destf[ARRAY_SIZE];
  static double array_destd[ARRAY_SIZE];

  signed int array_source_i[ARRAY_SIZE];
  signed long array_source_l[ARRAY_SIZE];

  for (int i = 0; i < ARRAY_SIZE; i++)
    {
      array_source_i[i] = VAL1;
      array_source_l[i] = VAL2;
    }

  vcvtf_32 (array_destf, array_source_i, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_destf[i] != (float) VAL1)
      {
	fprintf (stderr,"%d: %f != %f\n", i, array_destf[i], (float) VAL1);
	exit (1);
      }

  vcvtf_64 (array_destd, array_source_l, ARRAY_SIZE);
  for (int i = 0; i < ARRAY_SIZE; i++)
    if (array_destd[i] != (double) VAL2)
      {
	fprintf (stderr,"%d: %lf != %f\n", i, array_destd[i], (double) VAL2);
	exit (1);
      }

  return 0;
}
