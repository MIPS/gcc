/* { dg-do compile } */ 
/* { dg-options "-O1 -floop-test -fdump-tree-lptest-details " } */

double u[1782225];

void foo (int N, double *res)
{
  int i, j;
  double sum = 0;

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
	sum = sum + u[i + 1335 * j];
      u[1336 * i] *= 2;
    }
  *res = sum;
}

