/* Expect dynamic arrays to be mapped on the accelerator via
   GOMP_MAP_0LEN_ARRAY.  */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int
main ()
{
  const int n = 1000;
  int *a, *b, *c, *d, i;

  d = (int *) 12345;
  a = (int *) malloc (sizeof (int) * n);
  b = (int *) malloc (sizeof (int) * n);
  c = (int *) malloc (sizeof (int) * n);

  for (i = 0; i < n; i++)
    {
      a[i] = -1;
      b[i] = i+1;
      c[i] = 2*(i+1);
    }

#pragma acc enter data create(a[0:n]) copyin(b[:n], c[:n])
#pragma acc parallel loop
  for (i = 0; i < n; ++i)
    {
      a[i] = b[i] + c[i] + *((int *)&d);
    }
#pragma acc exit data copyout(a[0:n]) delete(b[0:n], c[0:n])

  for (i = 0; i < n; i++)
    assert (a[i] == 3*(i+1) + 12345);

  free (a);
  free (b);
  free (c);

  return 0;
}
