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

      // push c
#pragma omp task firstprivate (c) lastprivate (x)
      {
	// pop c
	if ('A' <= c && c <= 'Z')
	  x = c - 'A' + 'a';
	else
	  x = c;
	// push x
      }
      // pop x

      // push x
#pragma omp task firstprivate (x)
      {
	// pop x
	out[--j] = x;
      }
    }
}

/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_create" 1 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_init" 1 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_post" 1 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_wait" 1 "ompexp"} } */ 
/* { dg-final { scan-tree-dump-times "__builtin_GOMP_sem_destroy" 1 "ompexp"} } */ 
/* { dg-final { cleanup-tree-dump "graphite" } } */
