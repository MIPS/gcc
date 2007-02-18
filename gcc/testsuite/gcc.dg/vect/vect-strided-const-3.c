/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include <stdio.h>
#include "tree-vect.h"

#define N 16 

int
main1 ()
{
  int i;
  unsigned int out[N*8];

  for (i = 0; i < N; i++)
    {
      out[i*8] = 8;
      out[i*8 + 1] = 15;
      out[i*8 + 2] = 2;
      out[i*8 + 3] = 3;
      out[i*8 + 4] = 5;
      out[i*8 + 5] = 52;
      out[i*8 + 6] = 150;
      out[i*8 + 7] = 51;
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (out[i*8] != 8 
         || out[i*8 + 1] != 15 
         || out[i*8 + 2] != 2
         || out[i*8 + 3] != 3
         || out[i*8 + 4] != 5
         || out[i*8 + 5] != 52
         || out[i*8 + 6] != 150
         || out[i*8 + 7] != 51)
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
  
