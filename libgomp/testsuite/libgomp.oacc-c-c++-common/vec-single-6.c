/* { dg-additional-options "-w" } */

#include <assert.h>
#include <stdbool.h>

#define NUM_GANGS 4096

/* Test multiple conditions in vector-single mode.  */

int
main (int argc, char *argv[])
{
  bool fizz[NUM_GANGS], buzz[NUM_GANGS], fizzbuzz[NUM_GANGS];
  int i;

  #pragma acc parallel copyout(fizz, buzz, fizzbuzz) \
		       num_gangs(NUM_GANGS) num_workers(1) vector_length(32)
  {
    int j;
    
    /* This loop and the one following must be distributed to available gangs
       in the same way to ensure data dependencies are not violated (hence the
       "static" clauses).  */
    #pragma acc loop gang(static:*)
    for (j = 0; j < NUM_GANGS; j++)
      fizz[j] = buzz[j] = fizzbuzz[j] = 0;
    
    #pragma acc loop gang(static:*)
    for (j = 0; j < NUM_GANGS; j++)
      {
	if ((j % 3) == 0 && (j % 5) == 0)
	  fizzbuzz[j] = 1;
	else
	  {
	    if ((j % 3) == 0)
	      fizz[j] = 1;
	    else if ((j % 5) == 0)
	      buzz[j] = 1;
	  }
      }
  }

  for (i = 0; i < NUM_GANGS; i++)
    {
      assert (fizzbuzz[i] == ((i % 3) == 0 && (i % 5) == 0));
      assert (fizz[i] == ((i % 3) == 0 && (i % 5) != 0));
      assert (buzz[i] == ((i % 3) != 0 && (i % 5) == 0));
    }

  return 0;
}
