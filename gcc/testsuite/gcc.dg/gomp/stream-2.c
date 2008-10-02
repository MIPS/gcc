// { dg-do compile }
/* { dg-options "-O2 -fopenmp -fdump-tree-ompexp-all" } */

void
foo (char *in, char *out, int n)
{
  char c, x, y;
  int i = n, j = n;

  while (i)
    {
      c = in[--i];

#pragma omp task firstprivate (c) lastprivate (x)
      {
	if ('A' <= c && c <= 'Z')
#pragma omp task firstprivate (c) lastprivate (x)
	  x = c - 'A' + 'a';
	else
#pragma omp task firstprivate (c) lastprivate (x)
	  x = c;
      }

#pragma omp task firstprivate (x)
      {
	out[--j] = x;
      }
    }
}

/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_create" 3 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_init" 3 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_post" 3 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_wait" 3 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_destroy" 3 "ompexp"} } */ 
/* { dg-final { cleanup-tree-dump "graphite" } } */
