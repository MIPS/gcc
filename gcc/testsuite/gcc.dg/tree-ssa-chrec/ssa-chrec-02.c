/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */

int main(void)
{
  int a;
  int b;
  int *c;
  for (a = 11; a < 50; a++)
    {
      /* The following loop runs exactly 9 times.  */
      for (b = 8; b < 50; b+=5)
        {
	  c[a + 5] = 5;
	  c[b] = 6;
	  a+=2;
        }
    }
}

/* The analyzer has to detect the following evolution functions:
   b  ->  {8, +, 5}_2
   a  ->  {{11, +, 19}_1, +, 2}_2
*/
/* { dg-final { diff-tree-dumps "scev" } } */

