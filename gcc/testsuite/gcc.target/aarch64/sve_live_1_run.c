/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

extern void abort(void);
#include <string.h>

#include "sve_live_1.c"

#define MAX 62
#define START 27

int main (void)
{
  int a[MAX];
  int i;

  memset (a, 0, MAX*sizeof (int));

  int ret = liveloop (START, MAX, a);

  if (ret != 89)
    abort ();

  for (i=0; i<MAX; i++)
    {
      if (a[i] != i+START+1)
	abort ();
    }
}