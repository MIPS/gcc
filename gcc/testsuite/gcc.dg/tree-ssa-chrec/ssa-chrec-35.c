/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions -fall-data-deps -fdump-all-data-deps" } */

#define L 100
#define M 100
#define N 100

int bar (float);

int foo (float A[][M][N])
{
  int i, j, k;
  
  for (i = 0; i < L; i++)
    for (j = 0; j < M; j++)
      for (k = 0; k < N; k++)
	A[i][j][j] = A[i][j][k];
  
  return bar (A[10][11][12]);
}

/* { dg-final { diff-tree-dumps "scev" } } */
/* { dg-final { diff-tree-dumps "alldd" } } */
