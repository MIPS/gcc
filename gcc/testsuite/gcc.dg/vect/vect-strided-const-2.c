/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include <stdio.h>
#include "tree-vect.h"

#define N 16 

int
main1 ()
{
  int i;
  unsigned short out[N*4];

  for (i = 0; i < N; i++)
    {
      out[i*4] = 8;
      out[i*4 + 1] = 15;
      out[i*4 + 2] = 2;
      out[i*4 + 3] = 3;
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (out[i*4] != 8 
         || out[i*4 + 1] != 15 
         || out[i*4 + 2] != 2
         || out[i*4 + 3] != 3)
	abort ();
    }

  return 0;
}

int main (void)
{
  int i;
  
  check_vect ();

  main1 ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Constant stores in interleaving" 1 "vect"  } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
  
