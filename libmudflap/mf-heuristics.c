
/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

   This file is part of GCC.
   
   XXX: libgcc license?
*/

#include <stdio.h>
#include "mf-runtime.h"

extern char _end;

static void
__mf_register_ro_sections ()
{
  char buf[512];
  char flags[4];
  void *low, *high;

  FILE *fp;

  TRACE_IN;

  fp = fopen ("/proc/self/maps", "r");
  if (!fp)
    {
      if (__mf_opts.trace_mf_calls)
	{
	  fprintf (stderr, "mf: (heuristics) cannot open /proc/self/maps\n");
	  fprintf (stderr, "mf: (heuristics) registering 00-%p\n", &_end);
	}
      __mf_register (0, &_end, __MF_TYPE_GUESS, 
		     "(heuristic) static 0-__end region");
      return;
    }
  
  while (fgets (buf, sizeof(buf), fp))
    {
      if (sscanf (buf, "%p-%p %4c", &low, &high, flags) == 3)
	{
	  if (flags[0] == 'r' &&
	      flags[1] == '-')
	    {
	      if (__mf_opts.trace_mf_calls)
		{
		  fprintf (stderr, "mf: (heuristics) registering static region %p-%p\n",
			  low, high);
		}
	      __mf_register (low, (high-low), __MF_TYPE_GUESS, 
			     "(heuristic) static read-only segment");
	    } 

	  /* This clause will register the stack as a guess. Splitting regions in the
	     stack is much more delicate than in the heap. We will not do it yet. */
	  /*
	    else if (flags[2] == 'x')
	    {
	      if (__mf_opts.trace_mf_calls)
		{
		  fprintf (stderr, "mf: (heuristics) registering executable region %p-%p\n",
			  low, high);
		}
	      __mf_register (low, (high-low), __MF_TYPE_GUESS, 
			     "(heuristic) static executable segment");
	    }
	  */
	  else
	    {
	      if (__mf_opts.trace_mf_calls)
		{		  
		  fprintf (stderr, "mf: (heuristics) skipping region %s",buf);
		}
	    }
	}
    }
  fclose (fp);
  TRACE_OUT;
}


void
__mf_init_heuristics ()
{
  TRACE_IN;
  __mf_register_ro_sections ();
  TRACE_OUT;
}
