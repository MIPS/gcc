/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions" } */

int 
foo (int *c)
{
  int i;
  int j = 10;
  
  for (i = 0; i < 5; i++)
    {
      for (j = 10;; j--)
	{
	  if (j == 0)
	    break;
	  
	  *(c + j) = *(c + j) - 1;
	}
    }
  
  return j;
}

/* j  ->  {10, +, -1}_2  */

/* { dg-final { diff-tree-dumps "scev" } } */
