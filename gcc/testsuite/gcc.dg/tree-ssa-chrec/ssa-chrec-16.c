/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main (void)
{
  int a = -100;
  int b = 2;
  int c = 3;
  int d = 4;
  
  while (d)
    {
      a += 23;
      d = a + d;
    }
}

/* a  ->  {-100, +, 23}_1
   d  ->  {4, +, {-77, +, 23}_1}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */
