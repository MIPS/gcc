/* { dg-do link } */
/* { dg-require-effective-target sync_int_long } */
/* { dg-final { simulate-thread } } */

/* Test that a load is not precomputed before an acquire.  */

#include <stdio.h>
#include "simulate-thread.h"

int flag=0;
int x = 0, y = 10, i = 0, k = -1;

__attribute__((noinline))
void simulate_thread_main()
{
  /* Test that the first load of x is not cached and reused in the second
     load of x.  */

  /* Note: Technically this first load of x/y is a data race.  See
     note on atomic-hoist-1.c.  */
  i = x + y;

  if (__atomic_load_n (&flag, __ATOMIC_ACQUIRE))
    {
      /* x here should not be reused from above.  */
      k = x;
    }
}

void simulate_thread_other_threads ()
{
  /* Once i has been calculated in thread 1, change the value of x.  */
  if (i != 0)
    {
      x = -1;
      flag = 1;
    }
}

int simulate_thread_step_verify ()
{
  return 0;
}

int simulate_thread_final_verify ()
{
  if (k != -1)
    {
      printf("FAIL: k != -1\n");
      return 1;
    }
  return 0;
}

main()
{
  simulate_thread_main ();
  simulate_thread_done ();
  return 0;
}
