/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

/* Speculative loop with two loads from global buffers which can be aligned,
   but does require peeling.  */

int a[500];
int b[500];

int
foo (int n)
{
  int i = 0;
  do
    i += 1;
  while (a[i] + b[i] < n);
  return i;
}

/* { dg-final { scan-tree-dump-not "loop versioned for vectorization to enhance alignment" "vect" } } */
/* { dg-final { scan-tree-dump "Alignment of access forced using peeling" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 4 bytes of ref" "vect" } } */
/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
