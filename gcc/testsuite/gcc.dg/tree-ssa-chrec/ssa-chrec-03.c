/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main(void)
{
  int a;
  int b;
  int *c;
  for (a = 11; a < 50; a++)
    {
      /* The following loop runs exactly 7 times.  */
      for (b = 8; b < 50; b+=5)
        {
	  c[a++] = 5;
	  c[b++] = 6;
        }
    }
}

/* The analyzer has to detect the following evolution functions:
   b  ->  {8, +, 6}_2
   a  ->  {{11, +, 8}_1, +, 1}_2
*/
/* { dg-final { diff-tree-dumps "scev" } } */

