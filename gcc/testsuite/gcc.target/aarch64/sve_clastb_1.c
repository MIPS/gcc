/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define N 32

/* Simple condition reduction.  */

int __attribute__ ((noinline, noclone))
condition_reduction (int *a, int min_v)
{
  int last = 66; /* High start value.  */

  for (int i = 0; i < N; i++)
    if (a[i] < min_v)
      last = i;

  return last;
}

/* { dg-final { scan-assembler {\tclastb\tw[0-9]+, p[0-7], w[0-9]+, z[0-9]+\.s} } } */
