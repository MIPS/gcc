/* Test if acc_copyin has present_or_ behavior.  */

/* { dg-do run { target openacc_nvidia_accel_selected } } */

#include <stdio.h>
#include <stdlib.h>
#include <openacc.h>

int
main (int argc, char **argv)
{
  const int N = 256;
  int i;
  unsigned char *h;

  h = (unsigned char *) malloc (N);

  for (i = 0; i < N; i++)
    {
      h[i] = i;
    }

  (void) acc_copyin (h, N);

  fprintf (stderr, "CheCKpOInT\n");
  (void) acc_copyin (h, N);

  free (h);

  return 0;
}

/* { dg-output "CheCKpOInT(\n|\r\n|\r).*" } */
