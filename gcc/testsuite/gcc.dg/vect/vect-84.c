/* { dg-require-effective-target vect_int } */

/* PR tree-optimization/19347 */

#include <stdarg.h>
#include "tree-vect.h"
#include <stdio.h>
#include <stdlib.h>

#define N 16

struct osmesa_context {
  unsigned int clearpixel;
  void *buffer;
};

struct osmesa_context *osmesa;

int main1 (void *p)
{
  unsigned int i, n, *ptr;

  n = N;

  ptr = (unsigned int *) osmesa->buffer;
  for (i = 0; i < n; i++) {
    *ptr++ = osmesa->clearpixel;
  }

  /* check results  */
  ptr = (unsigned int *) osmesa->buffer;
  for (i = 0; i < n; i++) {
    if (*ptr++ != osmesa->clearpixel)
	abort();
  }

  return 0;
}
       
int main (void)
{
  void *p;
  check_vect ();
    
  osmesa = calloc( 1, sizeof (struct osmesa_context) );  
  osmesa->buffer = calloc( 1, sizeof (unsigned int) * N );  
  osmesa->clearpixel = 5;
  return main1 (p);
}
          
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
  

