/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main (void)
{
  int a;
  int b;
  int c;
  
  /* Exercises the MINUS_EXPR.  */
  for (a = 100; a > 50; a--)
    {
      
    }
}

/* The analyzer has to detect the following evolution function:
   a  ->  {100, +, -1}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */

