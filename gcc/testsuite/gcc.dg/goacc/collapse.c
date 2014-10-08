/* { dg-do compile } */
/* { dg-options "-O2 -std=c99" } */

#include <string.h>

int
main (void)
{
  int l = 0;
  int b[3][3];

  memset (b, '\0', sizeof (b));

#pragma acc parallel copy(b[0:3][0:3]) copy(l)
    {
#pragma acc loop collapse(2) reduction(+:l)
	for (int i = 0; i < 2; i++)
	  for (int j = 0; j < 2; j++)
	    if (b[i][j] != 16)
		  l += 1;
    }

  return 0;
}
