/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include <stdio.h>
#include "tree-vect.h"

#define N 16
#define SH_SUM 210
#define CH_SUM 120

int main1 ()
{
  int i;
  unsigned short udata_sh[N] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28};
  unsigned char udata_ch[N] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  unsigned int sum = 0;

  for (i = 0; i < N; i++){
    sum += udata_sh[i];
  }

  /* check results:  */
  if (sum != SH_SUM)
    abort ();

  sum = 0;

  for (i = 0; i < N; i++){
    sum += udata_ch[i];
  }

  /* check results:  */
  if (sum != CH_SUM)
    abort ();
  return 0;
}

int main (void)
{ 
  check_vect ();
  
  return main1 ();
}

/* { dg-final { scan-tree-dump-times "vectorized 2 loops" 1 "vect" { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
