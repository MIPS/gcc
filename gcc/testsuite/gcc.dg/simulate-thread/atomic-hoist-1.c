/* { dg-do link } */
/* { dg-require-effective-target sync_int_long } */
/* { dg-final { simulate-thread } } */

/* Test that a hoist is not performed across an acquire barrier.  */

#include <stdio.h>
#include "simulate-thread.h"

int iteration = 0;
int flag1=1, flag2=1;
unsigned int x=1, y=2, i=0x1234, j=0x5678, a;


/* At each instruction, get a new X or Y to later verify that we have
   not reused a value incorrectly.  */
void simulate_thread_other_threads ()
{
  if (iteration == 0)
    x++;
  else
    y++;
}

/* Return true if error, otherwise 0.  */
int verify_result ()
{
  /* [i] should not equal [j], because that would mean that we hoisted
     [x] or [y] instead of loading them again.  */
  int fail = i == j;
  if (fail)
    printf("FAIL: i (%u) should not equal j (%u)\n", i, j);
  return fail;
}

int simulate_thread_step_verify ()
{
  return verify_result ();
}

int simulate_thread_final_verify ()
{
  return verify_result ();
}

__attribute__((noinline))
void simulate_thread_main()
{
  for (; iteration < 2; ++iteration)
    {
      /* The values of x or y should not be hoisted across reads of
	 flag[12].

	 For example, when the second load below synchronizes with
	 another thread, the synchronization is with a release, and
	 that release may cause a stored value of x/y to be flushed
	 and become visible.  So, for this case, it is incorrect for
	 CSE/CSA/and-others to hoist x or y above the load of
	 flag2.  */
      if (__atomic_load_n (&flag1, __ATOMIC_ACQUIRE))
	i = x + y;
      if (__atomic_load_n (&flag2, __ATOMIC_ACQUIRE))
	a = 10;
      /* NOTE: According to the standard we can assume that the
	 testcase is data race free, so if there is an unconditional
	 load of x+y here at j=x+y, there should not be any other
	 thread writing to x or y if we are indeed data race free.

	 This means that we are technically free to hoist x/y.
	 However, since depending on these undefined behaviors is
	 liable to get many confused, it is best to be conservative
	 with optimizations on atomics, hence the current test.  As
	 we become better versed in optimizations across atomics, we
	 can relax the optimizations a bit.  */
      j = x + y;

      /* Since x or y have been changing at each instruction above, i
	 and j should be different.  If they are the same, we have
	 hoisted something incorrectly.  */
    }

}

main()
{
  simulate_thread_main ();
  simulate_thread_done ();
  return 0;
}
