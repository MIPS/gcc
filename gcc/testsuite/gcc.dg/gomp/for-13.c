// At one point in development, a typo disabled the remapping of the
// for iteration variable as private.

// { dg-do compile }
// { dg-options "-fopenmp -fdump-tree-gimple" }

extern void bar(int);
void foo(void)
{
  int i;

#pragma omp parallel for default(none)
  for (i = 0; i < 10; i++)
    bar(i);
}

// { dg-final { scan-tree-dump-times "shared" 0 "gimple" } }
// { dg-final { cleanup-tree-dump "gimple" } }
