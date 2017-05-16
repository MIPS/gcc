/* { dg-do run } */
/* { dg-skip-if "" { *-*-* } { "*" } { "-DACC_MEM_SHARED=0" } } */

#include <string.h>
#include <stdlib.h>
#include <openacc.h>

int
main (int argc, char **argv)
{
  const int N = 256;
  int i, c;
  unsigned char *h;
  void *d1, *d2;

  h = (unsigned char *) malloc (N);

  for (i = 0; i < N; i++)
    h[i] = i;

  #pragma acc enter data create (h[0:N])
  #pragma acc enter data copyin (h[0:N])
  #pragma acc enter data copyin (h[0:N])
  #pragma acc enter data copyin (h[0:N])

  acc_update_self (h, N);
  for (i = 0, c = 0; i < N; i++)
    if (h[i] == i)
      c++;
  /* h[] should be filled with uninitialized device values,
     abort if it's not.  */
  if (c == N)
    abort ();

  for (i = 0; i < N; i++)
    h[i] = 0;

  #pragma acc parallel present(h[0:N])
  {
    for (i = 0; i < N; i++)
      h[i] = 111;
  }

  /* No actual copyout should happen.  */
  acc_copyout (h, N);
  for (i = 0; i < N; i++)
    if (h[i] != 0)
      abort ();

  #pragma acc exit data delete (h[0:N])
  /* This should not actually be deleted yet.  */
  if (!acc_is_present (h, N))
    abort ();

  #pragma acc exit data copyout (h[0:N]) finalize

  for (i = 0; i < N; i++)
    if (h[i] != 111)
      abort ();

  if (acc_is_present (h, N))
    abort ();

  free (h);
  return 0;
}
