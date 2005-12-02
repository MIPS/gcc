/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

#include <stdio.h>
#include <stdlib.h>

int main()
{
  int i;
  _Decimal32 d;

  for (d = 1.1df, i=0; d <= 1.5df; d += 0.1df, i++)
    ;

  if (i != 5)
    abort();
  
  return (0);
}
