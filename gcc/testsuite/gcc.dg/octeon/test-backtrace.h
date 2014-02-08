/* Helper to test backtrace.  Expect frames consecutively as defined in
   FUNCTIONS.

   Symbol info is only collected for FUNCTIONS.  If you have functions other
   than these in the executable and their address is not lower than the
   functions with the lowest address in FUNCTIONS then they will be
   misattributed to one of the functions in FUNCTIONS.  Beware of this since
   you might get false positives.

   If you only have library and crt functions besides FUNCTIONS and you expect
   at least three frames then you should be fine.  Worst case they will be
   misidentified as the highest address function in FUNCTIONS but the bt will
   still have to match the other two functions.  */

#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef OCTEON_TARGET
#include "cvmx.h"
#include "cvmx-interrupt.h"
#endif

#define STR1(X) #X
#define STR(X) STR1(X)
#define F(X) { X, STR (X) }
#define FEND { NULL, "???" }

typedef struct func
{
  void *addr;
  const char *name;
} func_t;

extern func_t functions[];

static unsigned n_functions;
static void **sorted_functions;

/* Find the index of function START in backtrace.  Start at index IDX.  */

static int
find_function (void *start, void *bt[], unsigned len, unsigned idx)
{
  unsigned i;
  void *end;

  for (i = 0, end = 0; i < n_functions; i++)
    if (start < sorted_functions[i])
      break;
  if (i == 0)
    return -1;
  if (i != n_functions)
    end = sorted_functions[i];

  for (;idx < len; idx++)
    if (start <= bt[idx]
	&& (!end || bt[idx] < end))
      return idx;

  return -1;
}

static void
check_backtrace (void)
{
  void *buffer[10];
  unsigned len, i;
  int f_idx; 

  len = backtrace (buffer, 10);
  if (!len)
    {
      puts ("no backtrace info");
      exit (1);
    }

  for (i = 0; i < len; i++)
      printf ("  #%d  %p\n", i, buffer[i]);
  putchar ('\n');

  /* We don't have symbol info for some functions so certain frames can be
     misattributed.  Look for the actual frame sequence.  */
  for (i = 0; i < len; i++)
    {
      unsigned j;
      int idx;

      f_idx = find_function (functions[0].addr, buffer, len, i);
      if (f_idx == -1)
	continue;
      for (j = 1, idx = f_idx + 1; j < n_functions; j++, idx++)
	if (find_function (functions[j].addr, buffer, len, idx) != idx)
	  goto cont;
      break;
 cont:;
    }
  if (i == len)
    {
      puts ("can't find the functions in bt");
      exit (1);
    }
  for (i = 0; i < f_idx; i++)
      printf ("  #%d  %p\n", i, buffer[i]);
  for (;i < f_idx + n_functions; i++)
    printf ("  #%d  %p %s (%p)\n", i, buffer[i],
	    functions[i - f_idx].name, functions[i - f_idx].addr);
  for (;i < len; i++)
      printf ("  #%d  %p\n", i, buffer[i]);

#ifdef OCTEON_TARGET
  /* Make it pass.  */
  write (1, "\n*** EXIT code 0\n", 17);
#endif
  exit (0);
}

static void
handler(uint64_t registers[32])
{
  puts ("Exception!");
  check_backtrace ();
}

static int
compare (const void *a, const void *b)
{
  void *a1 = *(void**)a;
  void *b1 = *(void**)b;
  if (a1 > b1)
    return 1;
  else if (a1 < b1)
    return -1;
  return 0;
}

static void
testbt_setup (void)
{
  unsigned i;

  /* Don't expect functions to be a complete here, find the end at run
     time.  */
  for (n_functions = 0;
       functions[n_functions].addr != NULL;
       n_functions++)
    ;

  sorted_functions = malloc (sizeof (sorted_functions[0]) * n_functions);
  for (i = 0; i < n_functions; i++)
    sorted_functions[i] = functions[i].addr;
  qsort (sorted_functions, n_functions, sizeof (sorted_functions[0]), compare);
#ifdef OCTEON_TARGET
  cvmx_interrupt_set_exception (handler);
#endif
}
