/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

extern void abort(void);
#include <string.h>
#include <stdio.h>

#include "sve_live_2.c"

#define MAX 193
#define START 84

int main (void)
{
  int a[MAX];
  char b[MAX];
  int i;

  memset (a, 0, MAX*sizeof (int));
  memset (b, 23, MAX*sizeof (char));

  int ret = liveloop (START, MAX, a, b);

  if (ret != 277)
    abort ();

  for (i=0; i<MAX; i++)
    {
      if (a[i] != 24)
	abort ();
    }
}