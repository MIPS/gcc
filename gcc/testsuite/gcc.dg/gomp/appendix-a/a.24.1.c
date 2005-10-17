/* { dg-do compile } */

#include <omp.h>
int x, y, z[1000];
#pragma omp threadprivate(x)
void
a24 (int a)
{
  const int c = 1;
  int i = 0;
#pragma omp parallel default(none) private(a) shared(z)
  {
    int j = omp_get_num_threads ();
    /* O.K. - j is declared within parallel region */
    /* O.K.  -  a is listed in private clause */
    /*       -  z is listed in shared clause */
    x = c;			/* O.K.  -  x is threadprivate */
    				/*       -  c has const-qualified type */
    z[i] = y;
    /* { dg-error "'i' not specified" "" { target *-*-* } 19 } */
    /* { dg-error "enclosing parallel" "" { target *-*-* } 11 } */
    /* { dg-error "'y' not specified" "" { target *-*-* } 19 }  */
    /* { dg-error "enclosing parallel" "" { target *-*-* } 11 } */
#pragma omp for firstprivate(y)
    for (i = 0; i < 10; i++)
      {
	z[i] = y;		/* O.K. - i is the loop iteration variable */
				/*      - y is listed in firstprivate clause */
      }
    z[i] = y;
    /* { dg-error "'i' not specified" "" { target *-*-* } 28 } */
    /* { dg-error "enclosing parallel" "" { target *-*-* } 11 } */
    /* { dg-error "'y' not specified" "" { target *-*-* } 28 }  */
    /* { dg-error "enclosing parallel" "" { target *-*-* } 11 } */
  }
}
