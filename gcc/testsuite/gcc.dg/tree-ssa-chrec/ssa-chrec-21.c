/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main ()
{
  int a = 3;
  int b = 2;
  
  while (b)
    {
      a *= 4;
      b *= a;
    }
}

/* a  ->  {3, *, 4}_1
   b  ->  {{2, *, 12}_1, *, 4}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */
