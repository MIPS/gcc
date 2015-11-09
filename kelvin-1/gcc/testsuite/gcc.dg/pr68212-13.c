/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-vectorize -funroll-loops --param  max-unroll-times=4 -fdump-rtl-loop2_unroll" } */

volatile int a;

/*
 * loop with multiple entries runtime iterations, probably should not unroll
 */
void foo(double *d, unsigned long int n, double d1, double d2) {
  unsigned long int i;

  if (a <= 0) {
    goto middle_of_loop;
  }

  for (i=0;i<n;i++) {

    if (a > 5)
      d[i] = d1;
    else
      d[i] = d2;

  middle_of_loop:
    if (a == 0) {
      double td;
      td = d1;
      d1 = d2;
      d2 = td;
    }
  }
}

/* { dg-final { scan-rtl-dump-not "Invalid sum" "loop2_unroll" } } */
