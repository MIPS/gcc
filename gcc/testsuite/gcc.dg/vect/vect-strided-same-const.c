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

  /* Vectorizable without interleaving.  */
  for (i = 0; i < N; i++)
    {
      out[i*8] = 8;
      out[i*8 + 1] = 8;
      out[i*8 + 2] = 8;
      out[i*8 + 3] = 8;
      out[i*8 + 4] = 8;
      out[i*8 + 5] = 8;
      out[i*8 + 6] = 8;
      out[i*8 + 7] = 8;
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (out[i*8] != 8 
         || out[i*8 + 1] != 8
         || out[i*8 + 2] != 8
         || out[i*8 + 3] != 8
         || out[i*8 + 4] != 8
         || out[i*8 + 5] != 8
         || out[i*8 + 6] != 8
         || out[i*8 + 7] != 8)
	abort ();
    }

  /* Not vectorizable without interleaving.  */ 
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

/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect"  { target vect_interleave } } } */
/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  { target {! vect_interleave } } } } */
/* { dg-final { scan-tree-dump-times "Same constant store in interleaving" 1 "vect"  } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
  
