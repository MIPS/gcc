/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions" } */


int main (void)
{
  int a = -100;
  int b = 2;
  int c = 3;
  
  while (a)
    {
      /* Exercises if-phi-nodes.  */
      if (a < 30)
	a += b;
      else
	a += c;
      
      b++;
      c++;
    }
}

/* The analyzer has to detect the following evolution function:
   a  ->  {-100, +, {[2, 3], +, 1}_1}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */



