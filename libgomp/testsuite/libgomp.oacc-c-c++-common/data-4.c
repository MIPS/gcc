/* { dg-do run } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-DACC_MEM_SHARED=0" } } */

#include <string.h>
#include <stdlib.h>
#include <openacc.h>

int
main (int argc, char **argv)
{
  const int N = 256;
  int i;
  unsigned char *h;
  void *d1, *d2;

  h = (unsigned char *) malloc (N);

  for (i = 0; i < N; i++)
    {
      h[i] = i;
    }

#pragma acc data create (h[0:N])
  {
    #pragma acc enter data create (h[0:N])
  }

  if (!acc_is_present (h, N))
    abort ();

#pragma acc exit data delete (h[0:N])

  if (acc_is_present (h, N))
    abort ();

  free (h);
  return 0;
}
