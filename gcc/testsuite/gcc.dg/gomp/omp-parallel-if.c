/* { dg-do compile } */

#include <omp.h>

int
foo (void)
{
  return 10;
}

main ()
{
  int A = 0;

  /* Malformed uses of 'if' and 'num_threads'.  */
  #pragma omp parallel if (foo () > 10) shared (A) if (foo () == 3)	/* { dg-error "at most one 'if'" } */
    {
      A = omp_get_num_threads ();
    }

  #pragma omp parallel if (foo () == 10) num_threads (3) shared (A) num_threads (20)	/* { dg-error "at most one 'num_threads'" } */
    {
      A = omp_get_num_threads ();
    }

  /* Valid uses of 'if' and 'num_threads'.  */
  #pragma omp parallel if (foo () == 10) num_threads (foo ()) shared (A)
    {
      A = omp_get_num_threads ();
    }
}
