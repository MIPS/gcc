#include <cvmx.h>
#include <stdio.h>

/* This test requires 2 cores but CN30XX has only one core, generate an error. */
#ifndef OCTEON_MODEL
#error OCTEON_MODEL is not defined
#endif

#if (OCTEON_MODEL == OCTEON_CN3010 \
	|| OCTEON_MODEL == OCTEON_CN3010_PASS1  \
	|| OCTEON_MODEL == OCTEON_CN3010_PASS1_1 \
	|| OCTEON_MODEL == OCTEON_CN3005 \
	|| OCTEON_MODEL == OCTEON_CN3005_PASS1 \
	|| OCTEON_MODEL == OCTEON_CN3005_PASS1_1)
#error CN3010/CN3005 has only one core
#endif

volatile int i = 100;

int
main ()
{
  cvmx_user_app_init ();
#ifdef BUFFERED
  fprintf (stderr, "Running with buffered I/O\n");
#else
  fprintf (stderr, "Running without buffered I/O\n");
#endif
  if (cvmx_get_core_num () == 0)
    i = 50;
  while (i--)
    ;
  fprintf (stderr, "Starting to spin.\n");
  while (1)
    {
      /* Give some chance for the ^c handler to break us while not
	 locking the console.  */
      cvmx_wait (5000000);
#ifdef BUFFERED
      printf ("\ntick");
#else
      fprintf (stderr, "tick\n");
#endif
    }
  return 0;
}
