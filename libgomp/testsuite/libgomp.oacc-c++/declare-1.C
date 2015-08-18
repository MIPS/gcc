/* { dg-do run { target openacc_nvidia_accel_selected } } */

template<class T>
T foo()
{
  T a = 0;
  #pragma acc declare create (a)

  #pragma acc parallel
  {
    a = 5;
  }

  return a;
}

int
main (void)
{
  int rc;

  rc = foo<int>();

  return rc;
}
