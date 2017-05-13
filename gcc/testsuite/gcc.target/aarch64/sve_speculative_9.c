/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

/* Speculative loop with two loads which cannot both be aligned.  */

int
foo (int *a, int *b, int n)
{
  int i = 0;
  do
    i += 1;
  while (a[i] + b[i] < n);
  return i;
}

/* { dg-final { scan-tree-dump "loop versioned for vectorization to enhance alignment" "vect" } } */
/* { dg-final { scan-tree-dump-not "Alignment of access forced using peeling" "vect" } } */
/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
