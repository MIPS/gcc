/* { dg-do compile } */ 
/* { dg-options "-O1 -fscalar-evolutions -fdump-scalar-evolutions" } */


int main(void)
{
  int a;
  int b;
  int c;
  
  for (a = 22; a < 50; a+=1)
    {
      for (b = 23; b < 50; b+=5)
	{
	  ++a;
	}
      /* The following stmt exercises the value of B on the exit of the loop.
	 In this case the value of B out of the loop is that of the evolution
	 function of B applied to the number of iterations the inner loop_2 runs.  
	 Value (B) = {23, +, 5}_2 (6) = 53.  */
      /* At this point, the variable A has the evolution function:
	 {{22, +, 6}_1, +, 1}_2.  */
      a = a + b;
      /* At this point, the variable A has the evolution function:
	 {{22, +, 59}_1, +, 1}_2.  */
      /* And finally the a+=1 from the FOR_STMT produces the evolution function:
	 {{22, +, 60}_1, +, 1}_2.  */
    }
}

/* The analyzer has to detect the following evolution functions:
   b  ->  {23, +, 5}_2
   a  ->  {{22, +, 60}_1, +, 1}_2
*/
/* { dg-final { diff-tree-dumps "scev" } } */
