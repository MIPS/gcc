
/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

   This file is part of GCC.
   
   XXX: libgcc license?
*/

#include <stdio.h>
#include "mf-runtime.h"

#ifdef _MUDFLAP
#error "Do not compile this file with -fmudflap!"
#endif

extern char _end;

static void
__mf_register_ro_sections ()
{
  char buf[512];
  char flags[4];
  void *low, *high;

  FILE *fp;

  TRACE_IN;

  if (LIKELY (__mf_opts.heur_proc_map == 1))
    {
      fp = fopen ("/proc/self/maps", "r");
      if (!fp)
	{
	  if (__mf_opts.trace_mf_calls)
	    {
	      fprintf (stderr, "mf: (heuristics) cannot open /proc/self/maps\n");
	      fprintf (stderr, "mf: (heuristics) registering 00-%p\n", &_end);
	    }
	  __mf_register ((uintptr_t) 0, (uintptr_t) &_end,
			 __MF_TYPE_GUESS, 
			 "(heuristic) static 0-__end region");
	  TRACE_OUT;
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
		  __mf_register ((uintptr_t) low, (uintptr_t) (high-low),
				 __MF_TYPE_GUESS, 
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
    }
  TRACE_OUT;
}


int 
__mf_heuristic_check (uintptr_t ptr, uintptr_t ptr_high)
{

  /* First heuristic is to check stack bounds */

  if (__mf_opts.stack_bound)
    {
      TRACE ("mf: stack bound check on %p - %p\n", 
	     ptr, ptr_high);
      
      uintptr_t stack_top_guess = (uintptr_t)__builtin_frame_address(0);
      const uintptr_t stack_segment_base = 0xC0000000;
      const uintptr_t stack_segment_top  = 0xBFFFC000;
      
      TRACE ("mf: stack estimated as %p - %p\n", 
	     stack_top_guess, stack_segment_base);
      
      if (LIKELY(ptr >= stack_segment_top && 
		 ptr <= stack_segment_base))
	{
	  if (UNLIKELY(ptr_high > stack_segment_base))
	    {
	      TRACE ("mf: ptr_high %p off bottom of stack %p\n",
		     ptr_high, stack_segment_base);
	      return 0;
	    }
	  if (UNLIKELY(ptr < stack_top_guess))
	    {
	      TRACE ("mf: ptr %p off top of stack %p\n",
		     ptr_high, stack_top_guess);
	      return 0;
	    }
	  return (ptr_high >= ptr);
	}            
    }
  TRACE ("mf: no heuristics validate extent %p - %p\n", 
	 ptr, ptr_high);
    return 0;
}

void
__mf_init_heuristics ()
{
  TRACE_IN;
  __mf_register_ro_sections ();
  TRACE_OUT;
}
