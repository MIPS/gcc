/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fno-tree-ch -fdump-tree-scev" } */


int main ()
{
  int a = 3;
  int b = 2;
  
  while (a)
    {
      b += 5;
      a += b;
      
      /* Exercises the add_invariant_expr_to_loop_evolution
	 (loop_num = 1, chrec_var = {3, +, 7, +, 5}, to_add = 2).
	 The result should be:  {3, +, 9, +, 5}.  */
      a += 2;
    }
}

/* 
   b  ->  {2, +, 5}_1
   a  ->  {3, +, {9, +, 5}_1}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */
