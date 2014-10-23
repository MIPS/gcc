/* { dg-do run } */
/* TODO:
   { dg-xfail-run-if "" { *-*-* } { "-DACC_DEVICE_TYPE_host=1" } { "" } } */

int
main(void)
{
#define I 5
/* TODO */
#ifdef ACC_DEVICE_TYPE_host_nonshm
# define N 1
#else
# define N 11
#endif
#define A 8

  int a = A;
  int s = I;

#pragma acc parallel vector_length(N)
  {
    int i;
#pragma acc loop reduction(+:s)
    for (i = 0; i < N; ++i)
      s += a;
  }

  if (s != I + N * A)
    __builtin_abort();

  return 0;
}
