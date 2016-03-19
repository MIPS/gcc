/* { dg-additional-options "-O2" } */

/* The reduction on sum could cause an ICE with a non-simple latch loop.   */

int printf (char const *, ...);

int
main ()
{
  int i;
  double a[1000], sum = 0;

  
#pragma acc kernels pcopyin(a[0:1000])
#pragma acc loop reduction(+:sum)
  for(int i=0; i<1000; i++) {
    sum += a[i];
  }

  printf ("%lf\n", sum);

  return 0;
}
