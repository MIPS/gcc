#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* this is a simplistic test driver for memory fault detection using
   mudflap. you include this file in your mudflapped test case and define a
   function called "test" which is your testcase. you should be running
   mudflap with --viol-abort since this driver traps SIGABRT to determine
   if things went correctly. */

#ifdef SHOULD_FAIL
static const int should_fail = 1;
#else 
#ifdef SHOULD_PASS
static const int should_fail = 0;
#else
#error please define SHOULD_PASS or SHOULD_FAIL
#endif
#endif

static void test ();

void
abort_handler (int _ignored)
{
  if (should_fail)
    {
      /* printf ("PASS \t %s aborted as expected\n", __FILE__); */
      exit (0);
    }
  else
    {
      /* printf ("FAIL \t %s aborted unexpectedly (should have completed)\n", __FILE__); */
      exit (1);
    }
}

int 
main ()
{
  struct sigaction act;
  sigset_t set;

  sigemptyset (&set);

  act.sa_handler = &abort_handler;
  act.sa_mask = set;
  act.sa_flags = 0; 
     
  sigaction (SIGABRT, &act, NULL);

  /* call user-supplied testcase */
  test ();

  if (should_fail)
    {
      /* printf ("FAIL \t %s completed unexpectedly (should have aborted)\n", __FILE__); */
      exit (1);
    }
  else
    {
      /* printf ("PASS \t %s completed as expected\n", __FILE__); */
      exit (0);
    }
}
