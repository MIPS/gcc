/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions" } */


int bar (void);

int foo (int x)
{
  int a = -100;
  int b = 2;
  
  while (b)
    {
      if (x)
	a += 3;
      else
	a += bar ();
      
      /* Exercises the case when one of the branches of the if-phi-node cannot
	 be determined: [-oo, +oo].  
	 Since the evolution function is too difficult to handle in the expanded 
	 form, we have to keep it in its symbolic form:  "b  ->  {2, +, a_1}_1".  */
      b += a;
    }
}

/* a  ->  {-100, +, [-oo, +oo]}_1
   b  ->  {2, +, a_1}_1
*/

/* { dg-final { diff-tree-dumps "scev" } } */
