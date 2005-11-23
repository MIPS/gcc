/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 128 

typedef struct {
   unsigned short a;
   unsigned short b;
} viterbi_struct;

int
main1 (viterbi_struct *arr)
{
  viterbi_struct *ptr = arr;
  viterbi_struct res[N];
  int i;

  for (i = 0; i < N; i++)
    {
      res[i].a = ptr->b - ptr->a;
      res[i].b = ptr->b + ptr->a;
      ptr++;
    }
  
  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (res[i].a != arr[i].b - arr[i].a
          || res[i].b != arr[i].a + arr[i].b)
	abort ();
    }

  return 0;
}

int main (void)
{
  int i;
  viterbi_struct arr[N];
  
  check_vect ();

  for (i = 0; i < N; i++)
    { 
      arr[i].a = i;
      arr[i].b = i * 2;
      if (arr[i].a == 178)
         abort(); 
    } 

  main1 (arr);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
