/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main(void)
{
  int a;
  int b;
  int c;
  for (a = 22; a < 50; a++)
    {
      for (b = 23; b < 50; b+=5)
	{
	  for (c = a; c < 100; c++)
	    {
	      
	    }
	}
    }
}

/* The analyzer has to detect the following evolution functions:
   a  ->  {22, +, 1}_1
   b  ->  {23, +, 5}_2
   c  ->  {{22, +, 1}_1, +, 1}_3
*/
/* { dg-final { diff-tree-dumps "scev" } } */
