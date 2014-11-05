/* { dg-do run } */

#include <stdlib.h>

int
main (int argc, char *argv[])
{
#define N 10
  char a[N];

  {
    int i;
    for (i = 0; i < N; ++i)
      a[i] = 0;
  }

#pragma acc data copyout (a)
  {
#pragma acc parallel /* will result in a "dummy frame" */ present (a)
    {
      int i;
      for (i = 0; i < N; ++i)
	a[i] = i;
    }
  }

  {
    int i;
    for (i = 0; i < N; ++i)
      if (a[i] != i)
	abort ();
  }

  return 0;
}
