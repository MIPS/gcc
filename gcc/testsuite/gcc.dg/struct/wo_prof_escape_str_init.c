/* { dg-do compile } */
/* { dg-do run } */

#include <stdlib.h>
typedef struct
{
  int a;
  int b;
}str_t;

#define N 2

str_t A[2] = {{1,1},{2,2}};

int
main ()
{
  int i;

  for (i = 0; i < N; i++)
    A[i].b = A[i].a;

  for (i = 0; i < N; i++)
    if (A[i].b != A[i].a)
      abort ();
  
  return 0;
}

/*--------------------------------------------------------------------------*/
/* { dg-final { scan-ipa-dump "has escaped...Type is used in an array" "struct_reorg" } } */
