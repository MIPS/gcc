/* { dg-do run } */

extern void abort ();

int i;

int main(void)
{
  int j;

#if 0
  i = -1;
  j = -2;
#pragma acc data copyin (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != 2 || j != 1)
    abort ();

  i = -1;
  j = -2;
#pragma acc data copyout (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();

  i = -1;
  j = -2;
#pragma acc data copy (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();

  i = -1;
  j = -2;
#pragma acc data create (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();
#endif

  i = -1;
  j = -2;
#pragma acc data present_or_copyin (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != 2 || j != 1)
    abort ();

#if 0
  i = -1;
  j = -2;
#pragma acc data present_or_copyout (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();
#endif

  i = -1;
  j = -2;
#pragma acc data present_or_copy (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();

#if 0
  i = -1;
  j = -2;
#pragma acc data present_or_create (i, j)
  {
    // TODO: check that variables have been mapped.
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();
#endif

#if 0
  i = -1;
  j = -2;
#pragma acc data present (i, j)
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();
#endif

#if 0
  i = -1;
  j = -2;
#pragma acc data
  {
    // TODO: check that variables have been mapped.
    if (i != -1 || j != -2)
      abort ();
    i = 2;
    j = 1;
    if (i != 2 || j != 1)
      abort ();
  }
  if (i != -1 || j != -2)
    abort ();
#endif

  return 0;
}
