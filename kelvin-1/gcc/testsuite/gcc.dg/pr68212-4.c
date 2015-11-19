/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-vectorize -funroll-loops --param  max-unroll-times=4 -fdump-rtl-loop2_unroll" } */

#include <stdio.h>

void foo (double *d, unsigned long int n)
{
  unsigned long int i;
  volatile unsigned int j;
  for (i=0;i<1000000;i++)
    {
      switch (i % 5)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	  /* frequency of this block should be 4 times the frequency
	     of case 4, but that might be too much to expect from
	     the compiler's analysis. */
	  fprintf (stderr, "value %d has remainder < 4\n", i);
	  break;
	  
	case 4:
	  fprintf (stderr, "value %d has remainder 4\n", i);
	  break;
	  
	default:
	  fprintf (stderr, "this code should not be reached\n");
	  break;
	}
    }
}

/* { dg-final { scan-rtl-dump-not "Invalid sum" "loop2_unroll" } } */
