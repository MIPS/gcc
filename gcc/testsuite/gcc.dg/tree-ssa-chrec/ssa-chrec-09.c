/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


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

/* This is what is commonly called a "mixer".  It whirls the data in a
   strongly connected component.  We expect the following evolution
   functions:

   e  ->  {5, +, c_13}_1
   d  ->  {4, +, {5, +, c_13}_1}_1
   c  ->  {3, +, {4, +, {5, +, c_13}_1}_1}_1
   b  ->  {2, +, {3, +, {4, +, {5, +, c_13}_1}_1}_1}_1
   a  ->  {-100, +, {2, +, {3, +, {4, +, {5, +, c_13}_1}_1}_1}_1}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */


