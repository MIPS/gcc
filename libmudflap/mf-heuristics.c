
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


#if 0
/* These are used by heur_argv_envp below.  */
static int system_argc;
static char **system_argv;
static char **system_envp;

/* Hack: intercept / override glibc's __init_misc routine to 
   kidnap the process startup argc/argv/envp values. */
void
__init_misc (int argc, char **argv, char **envp)
{
  system_argc = argc;
  system_argv = argv;
  system_envp = envp;
  /* XXX: would be nice to call over to the glibc __init_misc routine. */
}
#endif



/* Run some quick validation of the given region.  If successful, return non-zero.
   If the result is cacheworthy, return something positive. */
int 
__mf_heuristic_check (uintptr_t ptr, uintptr_t ptr_high)
{
  VERBOSE_TRACE ("mf: heuristic check\n");

  /* The first heuristic is to check stack bounds.  Since this is a
     transient condition and quick to check, don't cache its
     result. */
  if (__mf_opts.heur_stack_bound)
    {
      uintptr_t stack_top_guess = (uintptr_t)__builtin_frame_address(0);

      VERBOSE_TRACE ("mf: stack estimated as %08lx-%08lx\n", 
		     stack_top_guess, stack_segment_base);

      if (ptr_high <= stack_segment_base &&
	  ptr >= stack_top_guess &&
	  ptr_high >= ptr)
	{
	  return 1; /* uncacheable */
	}            
    }


  /* The second heuristic is to scan the range of memory regions
     listed in /proc/self/maps, a special file provided by the Linux
     kernel.  Its results may be cached, and in fact, a GUESS object
     may as well be recorded for interesting matching sections.  */
  if (__mf_opts.heur_proc_map)
    {
      /* Keep a record of seen records from /proc/self/map.  */
      enum { max_entries = 500 };
      struct proc_self_map_entry
      {
	uintptr_t low;
	uintptr_t high;
      };
      static struct proc_self_map_entry entry [max_entries];
      static unsigned entry_used [max_entries];

      /* Look for a known proc_self_map entry that may cover this
	 region.  If one exists, then this heuristic has already run,
	 and should not be run again.  The check should be allowed to
	 fail.  */
      unsigned i;
      unsigned deja_vu = 0;
      for (i=0; i<max_entries; i++)
	{
	  if (entry_used[i] &&
	      (entry[i].low <= ptr) &&
	      (entry[i].high >= ptr_high))
	    deja_vu = 1;
	}

      if (! deja_vu)
	{
	  /* Time to run the heuristic.  Rescan /proc/self/maps; update the
	     entry[] array; XXX: remove expired entries, add new ones.  
	     XXX: Consider entries that have grown (e.g., stack).  */
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
			  for (i=0; i<max_entries; i++)
			    {
			      if (! entry_used[i])
				{
				  entry[i].low = (uintptr_t) low;
				  entry[i].high = (uintptr_t) high;
				  entry_used[i] = 1;
				  break;
				}
			    }
			  
			  VERBOSE_TRACE ("mf: registering region #%d "
					 "%08lx-%08lx given %s",
					 i, (uintptr_t) low, (uintptr_t) high, buf);
			  
			  /* XXX: bad hack; permit __mf_register to do its job.  */
			  __mf_state = active;
			  __mf_register ((uintptr_t) low, (uintptr_t) (high-low),
					 __MF_TYPE_GUESS, 
					 "/proc/self/maps segment");
			  __mf_state = reentrant;
			  
			  return 0; /* undecided (tending to cachable) */
			} 
		    }
		}
	      fclose (fp);
	    }
	}
    }


  /* The third heuristic is to approve all accesses between _start and _end,
     which should include all text and initialized data.  */
  if (__mf_opts.heur_start_end)
    if (ptr >= (uintptr_t) & _start && ptr_high <= (uintptr_t) & _end)
      return 1; /* uncacheable */

#if 0
  /* The fourth heuristic adds the startup argv/environ strings to the
     list of GUESS regions.  */
  if (__mf_opts.heur_argv_environ)
    {
      static unsigned done;
      if (! done)
	{
	  DECLARE(size_t, strlen, const char *s);
	  done = 1;

	  /* Register the environment string sequence.  */
	  if (system_envp)
	    {
	      int envp_length = 0;
	      int i;
	      while (system_envp[envp_length]) envp_length ++;
	      if (envp_length)
		__mf_register ((uintptr_t) system_envp,
			       (uintptr_t) (sizeof (char*) * (envp_length + 1)),
			       __MF_TYPE_GUESS, "environ[]");

	      for (i=0; i<envp_length; i++)
		{
		  char * env = system_envp[i];
		  if (env)
		    {
		      unsigned len = CALL_REAL (strlen, env);
		      __mf_register ((uintptr_t) env,
				     (uintptr_t) len+1,
				     __MF_TYPE_GUESS,
				     "envp[i]");
		    }
		}
	    }

	  /* Register the argument vector. */
	  if (system_argv)
	    {
	      int i;

	      __mf_register ((uintptr_t) system_argv,
			     (uintptr_t) (sizeof (char *) * (system_argc + 1)),
			     __MF_TYPE_GUESS,
			     "argv");

	      for (i=0; i<system_argc; i++)
		{
		  char * arg = system_argv[i];
		  if (arg)
		    {
		      unsigned len = CALL_REAL (strlen, arg);
		      __mf_register ((uintptr_t) arg,
				     (uintptr_t) (len + 1),
				     __MF_TYPE_GUESS,
				     "argv[i]");
		    }
		}
	    }

	  return 0;
	}
    }
#endif

  return -1; /* hard failure */
}
