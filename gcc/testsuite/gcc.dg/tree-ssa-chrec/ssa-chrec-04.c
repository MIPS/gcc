/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main(void)
{
  int a;
  int b;
  
  for (a = 22; a < 50; a++)
    {
      for (b = 23; b < 50; b+=5)
	{
	  b = b + a;
	}
    }
}

/* The analyzer has to detect the following evolution functions:
   a  ->  {22, +, 1}_1
   b  ->  {23, +, {27, +, 1}_1}_2
   b  ->  {23, +, a_1 + 5}_2
*/
/* { dg-final { diff-tree-dumps "scev" } } */
