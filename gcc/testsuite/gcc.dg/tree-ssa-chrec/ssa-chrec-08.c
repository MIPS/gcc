/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main(void)
{
  int a = -100;
  int b = 2;
  int c = 3;
  int d = 4;
  
  while (a)
    {
      /* Exercises higher order polynomials.  */
      a = a + b;
      b = b + c;
      c = c + d;
    }
}

/* The analyzer has to detect the following evolution functions:
   c  ->  {3, +, 4}_1
   b  ->  {{2, +, {3, +, 4}_1}_1
   a  ->  {{{-100, +, {2, +, {3, +, 4}_1}_1}_1
*/
/* { dg-final { diff-tree-dumps "scev" } } */

