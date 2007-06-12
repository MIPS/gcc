/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include <stdio.h>
#include "tree-vect.h"

#define N 128 

int
main1 ()
{
  int i;
  unsigned short out[N*8];

  for (i = 0; i < N; i++)
    {
      out[i*4] = 8;
      out[i*4 + 1] = 18;
      out[i*4 + 2] = 28;
      out[i*4 + 3] = 38;
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (out[i*4] != 8 
         || out[i*4 + 1] != 18
         || out[i*4 + 2] != 28
         || out[i*4 + 3] != 38)
	abort ();
    }

  for (i = 0; i < N; i++)
    {
      out[i*8] = 8;
      out[i*8 + 1] = 7;
      out[i*8 + 2] = 81;
      out[i*8 + 3] = 28;
      out[i*8 + 4] = 18;
      out[i*8 + 5] = 85;
      out[i*8 + 6] = 5;
      out[i*8 + 7] = 4;
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (out[i*8] != 8
         || out[i*8 + 1] != 7
         || out[i*8 + 2] != 81
         || out[i*8 + 3] != 28
         || out[i*8 + 4] != 18
         || out[i*8 + 5] != 85
         || out[i*8 + 6] != 5
         || out[i*8 + 7] != 4)
        abort ();
    }



  return 0;
}

int main (void)
{
  check_vect ();

  main1 ();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect"  } } */
/* { dg-final { scan-tree-dump-times "vectorizing stmts using SLP" 2 "vect"  } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
  
