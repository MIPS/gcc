
/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

   This file is part of GCC.
   
   XXX: libgcc license?
*/

#include <stdio.h>
#include "mf-runtime.h"
#include "mf-impl.h"

#ifdef _MUDFLAP
#error "Do not compile this file with -fmudflap!"
#endif

/* XXX */
extern char _end;
extern char _start;
static const uintptr_t stack_segment_base = 0xC0000000;
static const uintptr_t stack_segment_top  = 0xBF800000;


static int
is_stack_address (uintptr_t addr)
{
  return 
    (addr <= stack_segment_base) &&
    (addr >= stack_segment_top);
}





/* Run some quick validation of the given region.  If successful, return non-zero.
   If the result is cacheworthy, return something positive. */
int 
__mf_heuristic_check (uintptr_t ptr, uintptr_t ptr_high)
{
  TRACE_IN;

  /* The first heuristic is to check stack bounds.  Since this is a
     transient condition and quick to check, don't cache its
     result. */
  if (__mf_opts.heur_stack_bound)
    {
      uintptr_t stack_top_guess = (uintptr_t)__builtin_frame_address(0);

      VERBOSE_TRACE ("mf: stack estimated as %p-%p\n", 
		     stack_top_guess, stack_segment_base);

      if (ptr_high <= stack_segment_base &&
	  ptr >= stack_top_guess &&
	  ptr_high >= ptr)
	{
	  TRACE_OUT;
	  return 1; /* uncacheable */
	}            
    }

  /* The second heuristic is to scan the range of memory regions
     listed in /proc/self/maps, a special file provided by the Linux
     kernel.  Its results may be cached, and in fact, a GUESS object
     may as well be recorded for interesting matching sections.  */

  if (__mf_opts.heur_proc_map)
    {
      char buf[512];
      char flags[4];
      void *low, *high;
      FILE *fp;

      fp = fopen ("/proc/self/maps", "r");
      if (fp)
	{
	  while (fgets (buf, sizeof(buf), fp))
	    {
	      if (sscanf (buf, "%p-%p %4c", &low, &high, flags) == 3)
		{
		  if ((uintptr_t) low <= ptr &&
		      (uintptr_t) high >= ptr_high)
		    {
		      VERBOSE_TRACE ("mf: registering region %p-%p given %s\n",
				     low, high, buf);
		      
		      /* XXX: bad hack; permit __mf_register to do its job.  */
		      __mf_state = active;
		      __mf_register ((uintptr_t) low, (uintptr_t) (high-low),
				     __MF_TYPE_GUESS, 
				     "/proc/self/maps segment");
		      __mf_state = reentrant;

		      TRACE_OUT;
		      return 0; /* undecided tending to cachable yes */
		    } 
		}
	    }
	  fclose (fp);
	}
    }

  /* The third heuristic is to approve all accesses between _start and _end,
     which should include all text and initialized data.  */

  if (__mf_opts.heur_start_end)
    {
      if (ptr >= (uintptr_t) & _start && ptr_high <= (uintptr_t) & _end)
	{
	  TRACE_OUT;
	  return 1; /* uncacheable */
	}
    }


  TRACE_OUT;
  return -1; /* hard failure */
}
