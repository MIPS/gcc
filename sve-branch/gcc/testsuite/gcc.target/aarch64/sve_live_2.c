/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -fno-tree-scev-cprop -march=armv8-a+sve --save-temps -fdump-tree-vect-details" } */

int
liveloop (int start, int n, int * __restrict__ x, char * __restrict__ y)
{
  int i = start;
  int j;

  for (j = 0; j < n; ++j)
    {
      i += 1;
      x[j] = y[j] + 1;
    }
  return i;
}

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Can't use a fully-masked loop because ncopies is greater than 1" 1 "vect" } } */
