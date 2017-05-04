/* Test OpenACC update if_present clause.  */

#include <assert.h>
#include <openacc.h>

void
t (int n, int a, int *c, int *d)
{
  int b, i;

#pragma acc parallel loop present(c[0:n]) copyout(d[0:n])
  for (i = 0; i < n; i++)
    d[i] = c[i];

  for (i = 0; i < n; i++)
    {
      assert (d[i] == a);
      c[i] = i;
    }

  a = 0;

#pragma acc update device(c[0:n], a) if_present

#pragma acc parallel loop reduction(+:a) present(c[0:n])
  for (i = 0; i < n; i++)
    a += c[i];

#pragma acc update host(a) if_present

  b = 0;
  for (i = 0; i < n; i++)
    b += i;

  assert (a == b);

#pragma acc enter data copyin (b)
#pragma acc parallel loop reduction (+:b)
  for (i = 0; i < n; i++)
    b += i;

#pragma acc update self(b)
  assert (2*a == b);
#pragma acc exit data delete(b)

#pragma acc parallel loop present(c[0:n])
  for (i = 0; i < n; i++)
    c[i] =  i;

#pragma acc update host (c[0:n])

#pragma acc parallel loop present(c[0:n])
  for (i = 0; i < n; i++)
    c[i] =  1000 + i;
}

int
main ()
{
  int n = 100;
  int a, i, x[n], y[n];

#pragma acc update self(a) if_present
#pragma acc update if_present device(x[20:10])

  a = -1;
  for (i = 0; i < n; i++)
    x[i] = a;

#pragma acc enter data copyin(x)
  t (n, a, x, y);

#pragma acc update host(x[10:20])

  for (i = 0; i < n; i++)
#if !ACC_MEM_SHARED
    if (i < 10 || i >= 30)
      assert (x[i] == i);
    else
#endif
      assert (x[i] == 1000 + i);

#pragma acc exit data delete(x)

  a = -1;
  for (i = 0; i < n; i++)
    x[i] = a;

#pragma acc data copy (x)
  {
    t (n, a, x, y);
  }

  return 0;
}
