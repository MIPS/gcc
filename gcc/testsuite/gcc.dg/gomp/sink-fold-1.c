/* { dg-do compile } */
/* { dg-options "-fopenmp -fdump-tree-omplower" } */

/* Test depend(sink) clause folding.  */

int i,j, N;

extern void bar();

void
funk ()
{
#pragma omp parallel for ordered(2)
  for (i=0; i < N; i++)
    for (j=0; j < N; ++j)
    {
/* We remove the (sink:i,j-2) by virtue of it the i+0.  The remaining
   clauses get folded with a GCD of -2 for `i' and a minimum of -1 for
   'j'.  */
#pragma omp ordered \
  depend(sink:i-8,j-1) \
  depend(sink:i, j-2) \
  depend(sink:i-4,j+3) \
  depend(sink:i-6,j+2)
        bar();
#pragma omp ordered depend(source)
    }
}

/* { dg-final { scan-tree-dump-times "omp ordered depend\\(sink:i-2,j-1\\)" 1 "omplower" } } */
