/* { dg-do run } */
/* { dg-options "std=gnu99" } */

#include <stdio.h>
extern void abort (void);

int main()
{
  _Decimal32 d;
  int i;

  for (d = 1.1, i=0; d <= 1.5; d+= 0.1, i++)
    printf("d = %f\n", d);

  if (i != 5)
    abort();
  
  return (0);

}
