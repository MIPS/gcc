/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions" } */


int main(void)
{
  int a = -100;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;
  
  while (a)
    {
      /* Exercises the cycle detector: a -> b -> (c -> d -> e -> c)*.  */
      a += b;
      b += c;
      c += d;
      d += e;
      e += c;
    }
}

/* For the moment the analyzer does not know how to analyse the mixers and 
   exchangers.  We expect the analyser to output a rough approximation:
   e  ->  [-oo, +oo]
   d  ->  [-oo, +oo]
   c  ->  [-oo, +oo]
   b  ->  {2, +, c_3}_1
   a  ->  {-100, +, b_2}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */


