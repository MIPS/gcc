/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

This file is part of GCC.
XXX: libgcc license?
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif
#include <assert.h>

#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#include "mf-runtime.h"
#include "mf-impl.h"


#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifdef _MUDFLAP
#error "Do not compile this file with -fmudflap!"
#endif

/* ------------------------------------------------------------------------ */
/* Utility macros */


/* Codes to describe the context in which a violation occurs. */

#define __MF_VIOL_UNKNOWN 0
#define __MF_VIOL_READ 1
#define __MF_VIOL_WRITE 2
#define __MF_VIOL_REGISTER 3
#define __MF_VIOL_UNREGISTER 4
#define __MF_VIOL_WATCH 5


/* Protect against recursive calls. */
#define BEGIN_RECURSION_PROTECT           \
  enum __mf_state old_state;              \
  if (UNLIKELY (__mf_state == reentrant)) \
    return;                               \
  old_state = __mf_state;                 \
  __mf_state = reentrant;

#define END_RECURSION_PROTECT             \
  __mf_state = old_state;


/* ------------------------------------------------------------------------ */
/* Required globals.  */

#ifndef LOOKUP_CACHE_MASK_DFL
#define LOOKUP_CACHE_MASK_DFL 1023
#endif
#ifndef LOOKUP_CACHE_SIZE_MAX
#define LOOKUP_CACHE_SIZE_MAX 4096 /* Allows max CACHE_MASK 0x0FFF */
#endif
#ifndef LOOKUP_CACHE_SHIFT_DFL
#define LOOKUP_CACHE_SHIFT_DFL 2
#endif

struct __mf_cache __mf_lookup_cache [LOOKUP_CACHE_SIZE_MAX];
uintptr_t __mf_lc_mask = LOOKUP_CACHE_MASK_DFL;
unsigned char __mf_lc_shift = LOOKUP_CACHE_SHIFT_DFL;
#define LOOKUP_CACHE_SIZE (__mf_lc_mask + 1)

struct __mf_options __mf_opts;
enum __mf_state __mf_state = inactive;
#ifdef PIC
struct __mf_dynamic __mf_dynamic;
#endif

/* ------------------------------------------------------------------------ */
/* Configuration engine */

static void
__mf_set_default_options ()
{
  memset (& __mf_opts, 0, sizeof (__mf_opts));

  __mf_opts.trace_mf_calls = 0;
  __mf_opts.verbose_trace = 0;
  __mf_opts.collect_stats = 0;
  __mf_opts.internal_checking = 0;
  __mf_opts.tree_aging =    13037;
  __mf_opts.adapt_cache = 1000003;
  __mf_opts.print_leaks = 0;
  __mf_opts.abbreviate = 1;
  __mf_opts.check_initialization = 1;
  __mf_opts.verbose_violations = 1;
  __mf_opts.multi_threaded = 0;
  __mf_opts.free_queue_length = 4;
  __mf_opts.persistent_count = 100;
  __mf_opts.crumple_zone = 32;
  __mf_opts.backtrace = 4;
  __mf_opts.mudflap_mode = mode_check;
  __mf_opts.violation_mode = viol_nop;
  __mf_opts.heur_proc_map = 0;
  __mf_opts.heur_stack_bound = 0;
  __mf_opts.heur_start_end = 0;
  __mf_opts.heur_argv_environ = 1;
}

static struct option
{
  char *name;
  char *description;
  enum
    {
      set_option,
      read_integer_option,
    } type;
  int value;
  int *target;
} 
options [] =
  {
    {"mode-nop", 
     "mudflaps do nothing", 
     set_option, (int)mode_nop, (int *)&__mf_opts.mudflap_mode},    
    {"mode-populate", 
     "mudflaps populate object tree", 
     set_option, (int)mode_populate, (int *)&__mf_opts.mudflap_mode},    
    {"mode-check", 
     "mudflaps check for memory violations",
     set_option, (int)mode_check, (int *)&__mf_opts.mudflap_mode},
    {"mode-violate", 
     "mudflaps always cause violations (diagnostic)",
     set_option, (int)mode_violate, (int *)&__mf_opts.mudflap_mode},
   
    {"viol-nop", 
     "violations do not change program execution",
     set_option, (int)viol_nop, (int *)&__mf_opts.violation_mode},
    {"viol-abort", 
     "violations cause a call to abort()",
     set_option, (int)viol_abort, (int *)&__mf_opts.violation_mode},
    {"viol-segv", 
     "violations are promoted to SIGSEGV signals",
     set_option, (int)viol_segv, (int *)&__mf_opts.violation_mode},
    {"viol-gdb", 
     "violations fork a gdb process attached to current program",
     set_option, (int)viol_gdb, (int *)&__mf_opts.violation_mode},
    {"trace-calls", 
     "trace calls to mudflap runtime library",
     set_option, 1, &__mf_opts.trace_mf_calls},
    {"verbose-trace", 
     "trace internal events within mudflap runtime library",
     set_option, 1, &__mf_opts.verbose_trace},
    {"collect-stats", 
     "collect statistics on mudflap's operation",
     set_option, 1, &__mf_opts.collect_stats},
    {"internal-checking", 
     "perform more expensive internal checking",
     set_option, 1, &__mf_opts.internal_checking},
    {"age-tree", 
     "age the object tree after N accesses for working set",
     read_integer_option, 1000000, &__mf_opts.tree_aging},
    {"print-leaks", 
     "print any memory leaks at program shutdown",
     set_option, 1, &__mf_opts.print_leaks},
    {"check-initialization", 
     "detect uninitialized object reads",
     set_option, 1, &__mf_opts.check_initialization},
    {"verbose-violations", 
     "print verbose messages when memory violations occur",
     set_option, 1, &__mf_opts.verbose_violations},
    {"abbreviate", 
     "abbreviate repetitive listings",
     set_option, 1, &__mf_opts.abbreviate},
    /* XXX: this should be sensitive to gcc --enable-threading= setting */
    {"multi-threaded", 
     "support multiple threads",
     set_option, 1, &__mf_opts.multi_threaded},
    {"wipe-stack",
     "wipe stack objects at unwind",
     set_option, 1, &__mf_opts.wipe_stack},
    {"wipe-heap",
     "wipe heap objects at free",
     set_option, 1, &__mf_opts.wipe_heap},
    {"heur-proc-map", 
     "support /proc/self/map heuristics",
     set_option, 1, &__mf_opts.heur_proc_map},
    {"heur-stack-bound",
     "enable a simple upper stack bound heuristic",
     set_option, 1, &__mf_opts.heur_stack_bound},
    {"heur-start-end", 
     "support _start.._end heuristics",
     set_option, 1, &__mf_opts.heur_start_end},
    {"heur-argv-environ", 
     "support argv/environ heuristics",
     set_option, 1, &__mf_opts.heur_argv_environ},
     
    {"free-queue-length", 
     "queue N deferred free() calls before performing them",
     read_integer_option, 0, &__mf_opts.free_queue_length},
    {"persistent-count", 
     "keep a history of N unregistered regions",
     read_integer_option, 0, &__mf_opts.persistent_count},
    {"crumple-zone", 
     "surround allocations with crumple zones of N bytes",
     read_integer_option, 0, &__mf_opts.crumple_zone},
    {"lc-mask", 
     "set lookup cache size mask to N (2**M - 1)",
     read_integer_option, 0, (int *)(&__mf_lc_mask)},
    {"lc-shift", 
     "set lookup cache pointer shift",
     read_integer_option, 0, (int *)(&__mf_lc_shift)},
    {"lc-adapt", 
     "adapt mask/shift parameters after N cache misses",
     read_integer_option, 1, &__mf_opts.adapt_cache},
    {"backtrace", 
     "keep an N-level stack trace of each call context",
     read_integer_option, 0, &__mf_opts.backtrace},

    {0, 0, set_option, 0, NULL}
  };

static void
__mf_usage ()
{
  struct option *opt;

  fprintf (stderr, 
	   "This is a GCC \"mudflap\" memory-checked binary.\n"
	   "Mudflap is Copyright (C) 2002 Free Software Foundation, Inc.\n"
	   "\n"
	   "The mudflap code can be controlled by an environment variable:\n"
	   "\n"
	   "$ export MUDFLAP_OPTIONS='<options>'\n"
	   "$ <mudflapped_program>\n"
	   "\n"
	   "where <options> is a space-separated list of \n"
	   "any of the following options.  Use `-no-OPTION' to disable options.\n"
	   "\n");

  for (opt = options; opt->name; opt++)
    {
      int default_p = (opt->value == * opt->target);

      switch (opt->type)
	{
	  char buf[128];
	case set_option:
	  fprintf (stderr, "-%-23.23s %s", opt->name, opt->description);
	  if (default_p)
	    fprintf (stderr, " [default]\n");
	  else
	    fprintf (stderr, "\n");
	  break;
	case read_integer_option:
	  strncpy (buf, opt->name, 128);
	  strncpy (buf + strlen (opt->name), "=N", 2);
	  fprintf (stderr, "-%-23.23s %s", buf, opt->description);
	  fprintf (stderr, " [%d]\n", * opt->target);
	  break;	  
	default: abort();
	}
    }

  fprintf (stderr, "\n");
}


int 
__mf_set_options (const char *optstr)
{
  struct option *opts = 0;
  char *nxt = 0;
  long tmp = 0;
  int rc = 0;
  const char *saved_optstr = optstr;

  /* XXX: bounds-check for optstr! */

  while (*optstr)
    {
      switch (*optstr) {
      case ' ':
      case '\t':
      case '\n':
	optstr++;
	break;

      case '-':
	if (*optstr+1)
	  {	    
	    int negate = 0;
	    optstr++;

	    if (*optstr == '?' || 
		strncmp (optstr, "help", 4) == 0)
	      {
		/* Caller will print help and exit.  */
		rc = -1;
		break;
	      }
	    
	    if (strncmp (optstr, "no-", 3) == 0)
	      {
		negate = 1;
		optstr = & optstr[3];
	      }
	    
	    for (opts = options; opts->name; opts++)
	      {
		if (strncmp (optstr, opts->name, strlen (opts->name)) == 0)
		  {
		    optstr += strlen (opts->name);
		    assert (opts->target);
		    switch (opts->type) 
		      {
		      case set_option:
			if (negate)
			  *(opts->target) = 0;
			else
			  *(opts->target) = opts->value;
			break;
		      case read_integer_option:
			if (! negate && (*optstr == '=' && *(optstr+1)))
			  {
			    optstr++;
			    tmp = strtol (optstr, &nxt, 10);
			    if ((optstr != nxt) && (tmp != LONG_MAX))
			      {
				optstr = nxt;				
				*(opts->target) = (int)tmp;
			      }
			  }
			else if (negate)
			  * opts->target = 0;
			break;
		      }
		  }
	      }
	  }
	break;
	
      default:
	fprintf (stderr, 
		 "warning: unrecognized string '%s' in mudflap options\n",
		 optstr);
	optstr += strlen (optstr);
	rc = -1;
	break;
      }
    }

  /* Special post-processing: bound __mf_lc_mask and free_queue_length for security. */
  __mf_lc_mask &= (LOOKUP_CACHE_SIZE_MAX - 1);
  __mf_opts.free_queue_length &= (__MF_FREEQ_MAX - 1);

  /* Clear the lookup cache, in case the parameters got changed.  */
  memset (__mf_lookup_cache, 0, sizeof(__mf_lookup_cache));
  /* void slot 0 */
  __mf_lookup_cache[0].low = MAXPTR;

  TRACE ("mf: set options from `%s'\n", saved_optstr);

  return rc;
}

#define CTOR  __attribute__ ((constructor))
#define DTOR  __attribute__ ((destructor))

#ifdef PIC

static void 
resolve_single_dynamic (void **target, const char *name)
{
  char *err;
  assert (target);
  *target = NULL;
  *target = dlsym (RTLD_NEXT, name);
  err = dlerror ();
  if (err)
    {
      fprintf (stderr, "mf: error in dlsym(\"%s\"): %s\n",
	       name, err);
      abort ();
    }  
  if (! *target)
    {
      fprintf (stderr, "mf: dlsym(\"%s\") = NULL\n", name);
      abort ();
    }
}

void 
__mf_resolve_dynamics () 
{
#define RESOLVE(fname) \
resolve_single_dynamic (&__mf_dynamic.dyn_ ## fname, #fname)
  RESOLVE(calloc);
  RESOLVE(dlopen);
  RESOLVE(free);
  RESOLVE(malloc);
  RESOLVE(mmap);
  RESOLVE(munmap);
  RESOLVE(realloc);
#undef RESOLVE
}

#endif /* PIC */



/* ------------------------------------------------------------------------ */
/* stats-related globals.  */

static unsigned long __mf_count_check;
static unsigned long __mf_lookup_cache_reusecount [LOOKUP_CACHE_SIZE_MAX];
static unsigned long __mf_treerot_left, __mf_treerot_right;
static unsigned long __mf_count_register;
static unsigned long __mf_total_register_size [__MF_TYPE_MAX+1];
static unsigned long __mf_count_unregister;
static unsigned long __mf_total_unregister_size;
static unsigned long __mf_count_violation [__MF_VIOL_WATCH+1];


/* ------------------------------------------------------------------------ */
/* mode-check-related globals.  */

typedef struct __mf_object
{
  uintptr_t low, high; /* __mf_register parameters */
  const char *name;
  char type; /* __MF_TYPE_something */
  char watching_p; /* Trigger a VIOL_WATCH on access? */
  unsigned read_count; /* Number of times __mf_check/read was called on this object.  */
  unsigned write_count; /* Likewise for __mf_check/write.  */
  unsigned liveness; /* A measure of recent checking activity.  */
  unsigned description_epoch; /* Last epoch __mf_describe_object printed this.  */

  uintptr_t alloc_pc;
  struct timeval alloc_time;
  char **alloc_backtrace;
  size_t alloc_backtrace_size;

  int deallocated_p;
  uintptr_t dealloc_pc;
  struct timeval dealloc_time;
  char **dealloc_backtrace;
  size_t dealloc_backtrace_size;
} __mf_object_t;


typedef struct __mf_object_tree
{
  __mf_object_t data;
  struct __mf_object_tree *left;
  struct __mf_object_tree *right;
} __mf_object_tree_t;

/* Live objects: binary tree on __mf_object_t.low */
__mf_object_tree_t *__mf_object_root;
/* Dead objects: circular arrays; _MIN_CEM .. _MAX_CEM only */
unsigned __mf_object_dead_head[__MF_TYPE_MAX_CEM+1]; /* next empty spot */
__mf_object_tree_t *__mf_object_cemetary[__MF_TYPE_MAX_CEM+1][__MF_PERSIST_MAX];

static unsigned __mf_find_objects (uintptr_t ptr_low, uintptr_t ptr_high, 
				   __mf_object_tree_t **objs, unsigned max_objs);
static unsigned __mf_find_dead_objects (uintptr_t ptr_low, uintptr_t ptr_high, 
					__mf_object_tree_t **objs, unsigned max_objs);
static void __mf_link_object (__mf_object_tree_t *obj);
static void __mf_age_tree (__mf_object_tree_t *obj);
static void __mf_adapt_cache ();
static void __mf_unlink_object (__mf_object_tree_t *obj);
static void __mf_describe_object (__mf_object_t *obj);
static unsigned __mf_watch_or_not (void *ptr, size_t sz, char flag);



/* ------------------------------------------------------------------------ */

extern void __mf_init () CTOR;
void __mf_init ()
{
  char *ov = 0;

  __mf_state = starting;

#ifdef PIC
  __mf_resolve_dynamics ();
#endif

  __mf_set_default_options ();

  ov = getenv ("MUDFLAP_OPTIONS");
  if (ov)
    {
      int rc = __mf_set_options (ov);
      if (rc < 0)
	{
	  __mf_usage ();
	  exit (1);
	}
    }

  __mf_state = active;

  /* Initialize to a non-zero description epoch. */
  __mf_describe_object (NULL);

#define REG_RESERVED(obj) \
  __mf_register (& obj, sizeof(obj), __MF_TYPE_NOACCESS, # obj)

  REG_RESERVED (__mf_lookup_cache);
  REG_RESERVED (__mf_lc_mask);
  REG_RESERVED (__mf_lc_shift);
  /* XXX: others of our statics?  */

  /* Prevent access to *NULL. */
  __mf_register (MINPTR, 1, __MF_TYPE_NOACCESS, "NULL");
  __mf_lookup_cache[0].low = (uintptr_t) -1;

  /* XXX: bad hack: assumes Linux process layout */
  if (__mf_opts.heur_argv_environ)
    {
      int foo = 0;
      __mf_register (& foo,
		     (size_t) 0xC0000000 - (size_t) (& foo),
		     __MF_TYPE_GUESS,
		     "argv/environ area");
      /* XXX: separate heuristic? */
      __mf_register (& errno, sizeof (errno),
		     __MF_TYPE_GUESS,
		     "errno area");
    }
}


extern void __mf_fini () DTOR;
void __mf_fini ()
{
  TRACE ("mf: __mf_fini\n");
  __mf_report ();
}



/* ------------------------------------------------------------------------ */
/* __mf_check */

void __mf_check (void *ptr, size_t sz, int type, const char *location)
{
  unsigned entry_idx = __MF_CACHE_INDEX (ptr);
  struct __mf_cache *entry = & __mf_lookup_cache [entry_idx];
  int judgement = 0; /* 0=undecided; <0=violation; >0=okay */
  uintptr_t ptr_low = (uintptr_t) ptr;
  uintptr_t ptr_high = CLAMPSZ (ptr, sz);
  struct __mf_cache old_entry = *entry;

  BEGIN_RECURSION_PROTECT;
  TRACE ("mf: check ptr=%08lx size=%lu %s location=`%s'\n",
	 ptr, sz, (type == 0 ? "read" : "write"), location);
  
  switch (__mf_opts.mudflap_mode)
    {
    case mode_nop:
      judgement = 1;
      break;

    case mode_populate:
      entry->low = ptr_low;
      entry->high = ptr_high;
      judgement = 1;
      break;

    case mode_check:
      {
	unsigned heuristics = 0;

	/* Advance aging/adaptation counters.  */
	if (__mf_object_root)
	  {
	    static unsigned aging_count;
	    static unsigned adapt_count;
	    aging_count ++;
	    adapt_count ++;
	    if (UNLIKELY (__mf_opts.tree_aging > 0 &&
			  aging_count > __mf_opts.tree_aging))
	      {
		aging_count = 0;
		__mf_age_tree (__mf_object_root);
	      }
	    if (UNLIKELY (__mf_opts.adapt_cache > 0 &&
			  adapt_count > __mf_opts.adapt_cache))
	      {
		adapt_count = 0;
		__mf_adapt_cache ();
	      }
	  }

	/* Looping only occurs if heuristics were triggered.  */
	while (judgement == 0)
	  {
	    __mf_object_tree_t* ovr_obj[1];
	    unsigned obj_count;

	    obj_count = __mf_find_objects (ptr_low, ptr_high, ovr_obj, 1);

	    if (LIKELY (obj_count == 1)) /* A single hit! */
	      {
		__mf_object_t *obj = & ovr_obj[0]->data;
		assert (obj != NULL);
		if (LIKELY (ptr_low >= obj->low && ptr_high <= obj->high))
		  {
		    /* XXX: hope for no overflow!  */
		    if (type == __MF_CHECK_READ)
		      obj->read_count ++;
		    else
		      obj->write_count ++;

		    obj->liveness ++;
		    
		    if (UNLIKELY (obj->type == __MF_TYPE_NOACCESS))
		      judgement = -1;
		    else if (UNLIKELY (obj->watching_p))
		      judgement = -2; /* trigger VIOL_WATCH */
		    else if (UNLIKELY (__mf_opts.check_initialization
				       /* reading */
				       && type == __MF_CHECK_READ
				       /* not written */
				       && obj->write_count == 0
				       /* uninitialized (heap) */
				       && obj->type == __MF_TYPE_HEAP))
		      judgement = -1;
		    else
		      {
			/* Valid access.  */
			entry->low = obj->low;
			entry->high = obj->high;
			judgement = 1;
		      }
		  }
		/* The object did not cover the entire accessed region.  */
	      }
	    else if (LIKELY (obj_count > 1))
	      {
		__mf_object_tree_t **all_ovr_objs;
		unsigned n;
		DECLARE (void *, malloc, size_t c);
		DECLARE (void, free, void *p);

		all_ovr_objs = CALL_REAL (malloc, (sizeof (__mf_object_tree_t *) *
						   obj_count));
		if (all_ovr_objs == NULL) abort ();
		n = __mf_find_objects (ptr_low, ptr_high, all_ovr_objs, obj_count);
		assert (n == obj_count);

		/* Confirm that accessed range is covered by first/last object. */
		if (LIKELY ((ptr_low >= all_ovr_objs[0]->data.low) &&
			    (ptr_high <= all_ovr_objs[obj_count-1]->data.high)))
		  {
		    /* Presume valid access.  */
		    judgement = 1;

		    /* Confirm that intermediate objects are
		       contiguous and share a single name.  Thus they
		       are likely split up GUESS regions, or mmap
		       pages.  The idea of the name check is to
		       prevent an oversize access to a
		       stack-registered object (followed by some GUESS
		       type) from being accepted as a hit.  */
		    for (n=0; n<obj_count-1; n++)
		      {
			__mf_object_t *obj = & (all_ovr_objs[n]->data);
			__mf_object_t *nextobj = & (all_ovr_objs[n+1]->data);
			
			if (UNLIKELY (obj->type == __MF_TYPE_NOACCESS))
			  judgement = -1; /* Force error. */
			
			if (UNLIKELY (judgement == 1 &&
				      (obj->high + 1 != nextobj->low)))
			  judgement = 0; /* Cancel presumption. */
			
			if (UNLIKELY (judgement == 1 &&
				      (obj->name != nextobj->name)))
			  judgement = 0; /* Cancel presumption. */
			/* NB: strcmp above is not necessary since the
			   same literal string pointer is normally
			   used when creating regions.  */

			/* XXX: hope for no overflow!  */
			if (type == __MF_CHECK_READ)
			  obj->read_count ++;
			else
			  obj->write_count ++;
			obj->liveness ++;
		      }

		    /* If the access is otherwise successful, check whether
		       any of the covered objects are being watched.  */
		    if (judgement > 0)
		      {
			unsigned i;
			for (i=0; i<obj_count; i++)
			  if (all_ovr_objs[i]->data.watching_p)
			    judgement = -2;  /* trigger VIOL_WATCH */
		      }

		    /* Check for uninitialized reads.  */
		    if (judgement > 0 &&
			__mf_opts.check_initialization &&
			type == __MF_CHECK_READ)
		      {
			unsigned i;
			unsigned written_count = 0;

			for (i=0; i<obj_count; i++)
			  {
			    __mf_object_t *obj = & all_ovr_objs[i]->data;

			    if (obj->write_count
				|| obj->type == __MF_TYPE_HEAP_I
				|| obj->type == __MF_TYPE_GUESS)
			      written_count ++;
			  }
			
			/* Check for ALL pieces having been written-to.
			   XXX: should this be ANY instead?  */
			if (written_count != obj_count)
			  judgement = -1;
		      }

		    /* Fill out the cache with the bounds of the first
		       object and the last object that covers this
		       cache line (== includes the same __MF_CACHE_INDEX).
		       This could let this cache line span *two* distinct
		       registered objects: a peculiar but reasonable
		       situation.  The cache line may not include the
		       entire object though.  */
		    if (judgement > 0)
		      {
			unsigned i;
			entry->low = all_ovr_objs[0]->data.low;
			for (i=0; i<obj_count; i++)
			  {
			    uintptr_t high = all_ovr_objs[i]->data.high;
			    if (__MF_CACHE_INDEX (high) == entry_idx)
			      entry->high = high;
			  }
		      }
		  }

		CALL_REAL (free, all_ovr_objs);
	      }

	    if (judgement == 0)
	      {
		if (heuristics++ < 2) /* XXX parametrize this number? */
		  judgement = __mf_heuristic_check (ptr_low, ptr_high);
		else
		  judgement = -1;
	      }
	  }

      }
      break;

    case mode_violate:
      judgement = -1;
      break;
    }

  if (__mf_opts.collect_stats)
    {
      __mf_count_check ++;
      
      if (LIKELY (old_entry.low != entry->low || old_entry.high != entry->high))
	/* && (old_entry.low != 0) && (old_entry.high != 0)) */
	__mf_lookup_cache_reusecount [entry_idx] ++;    
    }
  
  END_RECURSION_PROTECT;
  
  if (UNLIKELY (judgement < 0))
    __mf_violation (ptr, sz,
		    (uintptr_t) __builtin_return_address (0), location,
		    ((judgement == -1) ? 
		     (type == __MF_CHECK_READ ? __MF_VIOL_READ : __MF_VIOL_WRITE) :
		     __MF_VIOL_WATCH));
}


static __mf_object_tree_t *
__mf_insert_new_object (uintptr_t low, uintptr_t high, int type, 
			const char *name, uintptr_t pc)
{
  DECLARE (void *, calloc, size_t c, size_t n);

  __mf_object_tree_t *new_obj;
  new_obj = CALL_REAL (calloc, 1, sizeof(__mf_object_tree_t));
  new_obj->data.low = low;
  new_obj->data.high = high;
  new_obj->data.type = type;
  new_obj->data.name = name;
  new_obj->data.alloc_pc = pc;
  gettimeofday (& new_obj->data.alloc_time, NULL);
  
  if (__mf_opts.backtrace > 0 && (type == __MF_TYPE_HEAP || type == __MF_TYPE_HEAP_I))
    new_obj->data.alloc_backtrace_size = 
      __mf_backtrace (& new_obj->data.alloc_backtrace,
		      (void *) pc, 2);
  
  __mf_link_object (new_obj);
  return new_obj;
}


static void 
__mf_uncache_object (__mf_object_t *old_obj)
{
  /* Remove any low/high pointers for this object from the lookup cache.  */
  
  /* Can it possibly exist in the cache?  */
  if (LIKELY (old_obj->read_count + old_obj->write_count))
    {
      uintptr_t low = old_obj->low;
      uintptr_t high = old_obj->high;
      unsigned idx_low = __MF_CACHE_INDEX (low);
      unsigned idx_high = __MF_CACHE_INDEX (high);
      unsigned i;
      for (i = idx_low; i <= idx_high; i++)
	{
	  struct __mf_cache *entry = & __mf_lookup_cache [i];
	  /* NB: the "||" in the following test permits this code to
	     tolerate the situation introduced by __mf_check over
	     contiguous objects, where a cache entry spans several 
	     objects.  */
	  if (entry->low == low || entry->high == high)
	    {
	      entry->low = MAXPTR;
	      entry->high = MINPTR;
	    }
	}
    }
}


void
__mf_register (void *ptr, size_t sz, int type, const char *name)
{
  /* if (UNLIKELY (!(__mf_state == active || __mf_state == starting))) return; */

  TRACE ("mf: register ptr=%08lx size=%lu type=%x name='%s'\n", ptr, sz, 
	type, name ? name : "");

  if (__mf_opts.collect_stats)
    {
      __mf_count_register ++;
      __mf_total_register_size [(type < 0) ? 0 :
				(type > __MF_TYPE_MAX) ? 0 : 
				type] += sz;
    }

  switch (__mf_opts.mudflap_mode)
    {
    case mode_nop:
      break;
      
    case mode_violate:
      __mf_violation (ptr, sz, (uintptr_t) __builtin_return_address (0), NULL,
		      __MF_VIOL_REGISTER);
      break;

    case mode_populate:
      /* Clear the cache.  */
      /* XXX: why the entire cache? */
      memset (__mf_lookup_cache, 0, sizeof(__mf_lookup_cache));
      /* void slot 0 */
      __mf_lookup_cache[0].low = MAXPTR;
      break;

    case mode_check:
      {
	__mf_object_tree_t *ovr_objs [1];
	unsigned num_overlapping_objs;
	uintptr_t low = (uintptr_t) ptr;
	uintptr_t high = CLAMPSZ (ptr, sz);
	uintptr_t pc = (uintptr_t) __builtin_return_address (0);
	
	BEGIN_RECURSION_PROTECT;
	
	/* Treat unknown size indication as 1.  */
	if (UNLIKELY (sz == 0)) sz = 1;
	
	num_overlapping_objs = __mf_find_objects (low, high, ovr_objs, 1);

	/* Handle overlaps.  */
	if (UNLIKELY (num_overlapping_objs > 0))
	  {
	    __mf_object_tree_t *ovr_obj = ovr_objs[0];
	    
	    /* Quietly accept a single duplicate registration for
	       static objects, since these may come from distinct
	       compilation units.  */
	    if (type == __MF_TYPE_STATIC
		&& ovr_obj->data.type == __MF_TYPE_STATIC
		&& ovr_obj->data.low == low
		&& ovr_obj->data.high == high)
	      {
		/* do nothing */
		VERBOSE_TRACE ("mf: duplicate static reg %08lx-%08lx `%s'\n", 
			       low, high, 
			       (ovr_obj->data.name ? ovr_obj->data.name : ""));
		END_RECURSION_PROTECT;
		return;
	      }

	    /* Quietly accept a single duplicate registration for
	       guess objects too.  */
	    if (type == __MF_TYPE_GUESS &&
		ovr_obj->data.type == __MF_TYPE_GUESS &&
		ovr_obj->data.low == low &&
		ovr_obj->data.high == high)
	      {
		/* do nothing */
		VERBOSE_TRACE ("mf: duplicate guess reg %08lx-%08lx\n", low, high);
		END_RECURSION_PROTECT;
		return;
	      }

	    /* Quietly accept new a guess registration that overlaps
	       at least one existing object.  Trim it down to size.  */
	    else if (type == __MF_TYPE_GUESS)
	      {
		/* We need to split this new GUESS region into some
		   smaller ones.  Or we may not need to insert it at
		   all if it is covered by the overlapping region.  */

		/* First, identify all the overlapping objects.  */
		__mf_object_tree_t **all_ovr_objs;
		unsigned num_ovr_objs, n;
		uintptr_t next_low;
		DECLARE (void *, malloc, size_t c);
		DECLARE (void, free, void *p);

		all_ovr_objs = CALL_REAL (malloc, (sizeof (__mf_object_tree_t *) *
						   num_overlapping_objs));
		if (all_ovr_objs == NULL) abort ();
		num_ovr_objs = __mf_find_objects (low, high, all_ovr_objs,
						  num_overlapping_objs);
		assert (num_ovr_objs == num_overlapping_objs);

		VERBOSE_TRACE ("mf: splitting guess %08lx-%08lx, # overlaps: %u\n",
			       low, high, num_ovr_objs);

		/* Add GUESS regions between the holes: before each
		   overlapping region.  */
		END_RECURSION_PROTECT;
		next_low = low;
		/* This makes use of the assumption that __mf_find_objects() returns
		   overlapping objects in an increasing sequence.  */
		for (n=0; n < min (num_ovr_objs, num_overlapping_objs); n++)
		  {
		    if (all_ovr_objs[n]->data.low > next_low) /* Gap? */
		      {
			uintptr_t next_high = CLAMPSUB (all_ovr_objs[n]->data.low, 1);
			__mf_register ((void *) next_low, next_high-next_low+1,
				       __MF_TYPE_GUESS, name);
		      }
		    next_low = CLAMPADD (all_ovr_objs[n]->data.high, 1);
		  }
		/* Add in any leftover room at the top.  */
		if (next_low <= high)
		  __mf_register ((void *) next_low, high-next_low+1,
				 __MF_TYPE_GUESS, name);

		/* XXX: future optimization: allow consecutive GUESS regions to
		   be glued together.  */
		CALL_REAL (free, all_ovr_objs);
		return;
	      }

	    /* Quietly accept a non-GUESS region overlaying a GUESS
	       region.  Handle it by removing the GUESS region
	       temporarily, then recursively adding this new object,
	       and then the GUESS back.  The latter will be split up
	       by the recursive process above.  */
	    else if (ovr_obj->data.type == __MF_TYPE_GUESS)
	      {
		uintptr_t old_low = ovr_obj->data.low;
		uintptr_t old_high = ovr_obj->data.high;
		const char* old_name = ovr_obj->data.name;

		/* Now to recursively remove the guess piece, and
  		   reinsert them in the opposite order.  Recursion
  		   should bottom out if another non-GUESS overlapping
  		   region is found for this new object (resulting in a
  		   violation), or if no further overlap occurs.  The
  		   located GUESS region should end up being split up
  		   in any case.  */
		END_RECURSION_PROTECT;
		__mf_unregister ((void *) old_low, old_high-old_low+1);
		__mf_register ((void *) low, sz, type, name);
		__mf_register ((void *) old_low, old_high-old_low+1,
			       __MF_TYPE_GUESS, old_name);
		return;
	      }

	    /* Alas, a genuine violation.  */
	    else
	      {
		/* Two or more *real* mappings here. */
		__mf_violation ((void *) ptr, sz,
				(uintptr_t) __builtin_return_address (0), NULL,
				__MF_VIOL_REGISTER);
	      }
	  }
	
	/* No overlapping objects: AOK.  */
	else
	  {
	    __mf_insert_new_object (low, high, type, name, pc);
	  }
	
	/* We could conceivably call __mf_check() here to prime the cache,
	   but then the read_count/write_count field is not reliable.  */
	
	END_RECURSION_PROTECT;
	break;
      }
    } /* end switch (__mf_opts.mudflap_mode) */
}



void
__mf_unregister (void *ptr, size_t sz)
{
  DECLARE (void, free, void *ptr);
  BEGIN_RECURSION_PROTECT;

  TRACE ("mf: unregister ptr=%08lx size=%lu\n", ptr, sz);

  switch (__mf_opts.mudflap_mode)
    { 
    case mode_nop:
      break;

    case mode_violate:
      __mf_violation (ptr, sz,
		      (uintptr_t) __builtin_return_address (0), NULL,
		      __MF_VIOL_UNREGISTER);
      break;

    case mode_populate:
      /* Clear the cache.  */
      memset (__mf_lookup_cache, 0, sizeof(__mf_lookup_cache));
      /* void slot 0 */
      __mf_lookup_cache[0].low = MAXPTR;
      break;

    case mode_check:
      {
	__mf_object_tree_t *old_obj = NULL;
	__mf_object_tree_t *del_obj = NULL;  /* Object to actually delete. */
	__mf_object_tree_t *objs[1] = {NULL};
	unsigned num_overlapping_objs;
		
	/* Treat unknown size indication as 1.  */
	if (sz == 0) sz = 1;
	
	num_overlapping_objs = __mf_find_objects ((uintptr_t) ptr,
						  CLAMPSZ (ptr, sz), objs, 1);

	/* XXX: handle unregistration of big old GUESS region, that has since
	   been splintered.  */

	if (UNLIKELY (num_overlapping_objs != 1))
	  {
	    /* XXX: also: should check/assert ptr == old_obj->low ? */
	    END_RECURSION_PROTECT;
	    __mf_violation (ptr, sz,
			    (uintptr_t) __builtin_return_address (0), NULL,
			    __MF_VIOL_UNREGISTER);
	    return;
	  }
	
	old_obj = objs[0];

	__mf_unlink_object (old_obj);
	__mf_uncache_object (& old_obj->data);

	/* Wipe buffer contents if desired.  */
	if ((__mf_opts.wipe_stack && old_obj->data.type == __MF_TYPE_STACK)
	    || (__mf_opts.wipe_heap && (old_obj->data.type == __MF_TYPE_HEAP 
					|| old_obj->data.type == __MF_TYPE_HEAP_I)))
	  {
	    memset ((void *) old_obj->data.low,
		    0,
		    (size_t) (old_obj->data.high - old_obj->data.low + 1));
	  }
	
	/* Manage the object cemetary.  */
	if (__mf_opts.persistent_count > 0 && 
	    old_obj->data.type != __MF_TYPE_GUESS)
	  {
	    old_obj->data.deallocated_p = 1;
	    old_obj->left = old_obj->right = NULL;
	    old_obj->data.dealloc_pc = (uintptr_t) __builtin_return_address (0);
	    gettimeofday (& old_obj->data.dealloc_time, NULL);
	    
	    if (__mf_opts.backtrace > 0 && old_obj->data.type == __MF_TYPE_HEAP)
	      old_obj->data.dealloc_backtrace_size = 
		__mf_backtrace (& old_obj->data.dealloc_backtrace,
				NULL, 2);

	    /* Encourage this object to be displayed again in current epoch.  */
	    old_obj->data.description_epoch --;

	    /* Put this object into the cemetary.  This may require this plot to
	       be recycled, and the previous resident to be designated del_obj.  */
	    
	    assert (old_obj->data.type >= 0 && 
		    old_obj->data.type <= __MF_TYPE_MAX_CEM);
	    {
	      unsigned row = old_obj->data.type;
	      unsigned plot = __mf_object_dead_head [row];
	      
	      del_obj = __mf_object_cemetary [row][plot];
	      __mf_object_cemetary [row][plot] = old_obj;

	      plot ++;
	      if (plot == __mf_opts.persistent_count) plot = 0;
	      __mf_object_dead_head [row] = plot;
	    }
	    
	  }
	else
	  del_obj = old_obj;
	
	if (__mf_opts.print_leaks)
	  {
	    if ((old_obj->data.read_count + old_obj->data.write_count) == 0 &&
		(old_obj->data.type == __MF_TYPE_HEAP 
		 || old_obj->data.type == __MF_TYPE_HEAP_I))
	      {
		fprintf (stderr, 
			 "*******\n"
			 "mudflap warning: unaccessed registered object:\n");
		__mf_describe_object (& old_obj->data);
	      }
	  }
	
	if (del_obj != NULL) /* May or may not equal old_obj.  */
	  {
	    if (__mf_opts.backtrace > 0)
	      {
		CALL_REAL(free, del_obj->data.alloc_backtrace);
		if (__mf_opts.persistent_count > 0)
		  {
		    CALL_REAL(free, del_obj->data.dealloc_backtrace);
		  }
	      }
	    CALL_REAL(free, del_obj);
	  }
	
	break;
      }
    } /* end switch (__mf_opts.mudflap_mode) */


  if (__mf_opts.collect_stats)
    {
      __mf_count_unregister ++;
      __mf_total_unregister_size += sz;
    }

  END_RECURSION_PROTECT;
}

/* ------------------------------------------------------------------------ */
/* __mf_validate_live_object_tree, _object_cemetary */

static void
__mf_validate_live_object_tree (__mf_object_tree_t *obj)
{
  assert (obj != NULL);

  if (__mf_opts.persistent_count > 0)
    assert (! obj->data.deallocated_p);

  if (obj->left)
    {
      assert (obj->left->data.high < obj->data.low);
      __mf_validate_live_object_tree (obj->left);
    }
  if (obj->right)
    {
      assert (obj->right->data.low > obj->data.high);
      __mf_validate_live_object_tree (obj->right);
    }
}

static void
__mf_validate_object_cemetary ()
{
  unsigned cls;
  unsigned i;

  for (cls = 0; cls <= __MF_TYPE_MAX_CEM; cls++)
    {
      assert (__mf_object_dead_head [cls] >= 0 &&
	      __mf_object_dead_head [cls] < __mf_opts.persistent_count);
      for (i = 0; i < __mf_opts.persistent_count; i++)
	{
	  __mf_object_tree_t *obj = __mf_object_cemetary [cls][i];
	  if (obj != NULL)
	    {
	      assert (obj->data.deallocated_p);
	      assert (obj->left == NULL);
	      assert (obj->right == NULL);
	    }
	}
    }
}

static void 
__mf_validate_objects ()
{
  if (__mf_object_root)
    __mf_validate_live_object_tree (__mf_object_root);

  if (__mf_opts.persistent_count > 0)
    __mf_validate_object_cemetary ();
}


static void
__mf_age_tree (__mf_object_tree_t *obj)
{
  assert (obj != NULL);
  obj->data.liveness = obj->data.liveness >> 1;

  if (obj->left)
    __mf_age_tree (obj->left);
  if (obj->right)
    __mf_age_tree (obj->right);
}



struct tree_stats
{
  unsigned obj_count;
  uintptr_t total_size;
  unsigned live_obj_count;
  double total_weight;
  double weighted_size;
};


static void
__mf_tree_analyze (__mf_object_tree_t *obj, struct tree_stats* s)
{
  assert (obj != NULL);

  /* Exclude never-accessed objects.  */
  if (obj->data.read_count + obj->data.write_count)
    {
      s->obj_count ++;
      s->total_size += (obj->data.high - obj->data.low + 1);

      if (obj->data.liveness)
	{
	  s->live_obj_count ++;
	  s->total_weight += (double) obj->data.liveness;
	  s->weighted_size +=
	    (double) (obj->data.high - obj->data.low + 1) *
	    (double) obj->data.liveness;
	}
    }

  if (obj->left)
    __mf_tree_analyze (obj->left, s);
  if (obj->right)
    __mf_tree_analyze (obj->right, s);
}


static void
__mf_adapt_cache ()
{
  struct tree_stats s;
  double avg_weight;
  uintptr_t avg_size;
  uintptr_t weighted_avg_size;
  uintptr_t new_mask;
  uintptr_t shifted;
  unsigned char new_shift;
  double cache_utilization;
  unsigned i;

  memset (&s, 0, sizeof (s));
  if (__mf_object_root)
    __mf_tree_analyze (__mf_object_root, & s);

  /* Maybe we're dealing with funny aging/adaptation parameters, or an
     empty tree.  Just leave the cache alone in such cases, rather
     than risk dying by division-by-zero.  */
  if (! (s.obj_count > 0) && (s.live_obj_count > 0) && (s.total_weight > 0.0))
    return;

  avg_weight = s.total_weight / s.live_obj_count;
  weighted_avg_size = (uintptr_t) (s.weighted_size / s.total_weight);
  avg_size = (uintptr_t) (s.total_size / s.obj_count);
  if (avg_size == 0) avg_size = 1;

  /* Count number of used buckets.  */
  cache_utilization = 0.0;
  for (i = 0; i < (1 + __mf_lc_mask); i++)
    if (__mf_lookup_cache[i].low != 0 || __mf_lookup_cache[i].high != 0)
      cache_utilization += 1.0;
  cache_utilization /= (1 + __mf_lc_mask);
  if (cache_utilization < 0.5)
    new_mask = __mf_lc_mask >> 1;
  else if (cache_utilization > 0.7)
    new_mask = (__mf_lc_mask << 1) | 1;
  else
    new_mask = __mf_lc_mask;
  
  new_mask |= 0x3ff; /* impose a practical minimum */
  new_mask &= (LOOKUP_CACHE_SIZE_MAX - 1);

#if 0 /* giving up on heuristics? */
  new_mask = __mf_lc_mask;
#endif

  /* Find a good new shift amount.  Make it big enough that the
     popular objects take up 1-2 "cache lines".  The 24 in the
     next line imposes a practical 16MB upper limit on the cache line
     size.  */
  for (new_shift=0, shifted=2; /* shifted == 2**(new_shift+1) */
       new_shift < 24;
       new_shift++, shifted<<=1)
    if (shifted > weighted_avg_size)
      break;
    
  VERBOSE_TRACE ("mf: adapt cache %u/%u/%lu/%.0f/%.0f => "
		 "%.0f/%lu/%lu/%.0f%% => "
		 "%08lx/%u\n",
		 s.obj_count, s.live_obj_count, s.total_size,
		 s.total_weight, s.weighted_size,
		 avg_weight, weighted_avg_size, avg_size, (cache_utilization*100.0),
		 new_mask, new_shift);

  /* We should reinitialize cache if its parameters have changed.  */
  if (new_mask != __mf_lc_mask ||
      new_shift != __mf_lc_shift)
    {
      __mf_lc_mask = new_mask;
      __mf_lc_shift = new_shift;
      memset (__mf_lookup_cache, 0, sizeof(__mf_lookup_cache));
      /* void slot 0 */
      __mf_lookup_cache[0].low = MAXPTR;
    }
}






/* __mf_find_object[s] */

/* Find overlapping live objecs between [low,high].  Return up to
   max_objs of their pointers in objs[].  Return total count of
   overlaps (may exceed max_objs). */

/* XXX: track traversal statistics, like average depth, balance.  */

static unsigned
__mf_find_objects_rec (uintptr_t low, uintptr_t high, __mf_object_tree_t **nodep,
		       __mf_object_tree_t **objs, unsigned max_objs)
{
  unsigned count;
  __mf_object_tree_t *node = *nodep;

  assert (low <= high);
  assert (max_objs == 0 || objs != NULL);

  if (UNLIKELY (node == NULL)) return 0;

  /* Traverse down left subtree. */
  count = 0;
  if (low < node->data.low)
    count += __mf_find_objects_rec (low, high, & node->left, objs, max_objs);

  /* Track the used slots of objs[].  */
  if (count < max_objs)
    {
      objs += count;
      max_objs -= count;
    }
  else
    {
      max_objs = 0;
    }

  /* Check for overlap with this node.  */
  if (high >= node->data.low && low <= node->data.high)
    {
      count ++;
      if (max_objs > 0)  /* Any room left?  */
	{
	  objs[0] = node;
	  objs ++;
	  max_objs --;
	}
    }

  /* Traverse down right subtree.  */
  if (high > node->data.high)
    count += __mf_find_objects_rec (low, high, & node->right, objs, max_objs);
  /* There is no need to manipulate objs/max_objs any further.  */


  /* Rotate a child node up if its access count is higher. */
  if (UNLIKELY ((node->left && node->left->data.liveness > node->data.liveness) &&
		((!node->right || (node->right && 
				   node->left->data.liveness > 
				   node->right->data.liveness)))))
    {
      __mf_object_tree_t *l = node->left;
      __mf_object_tree_t *l_r = l->right;

      *nodep = l;
      l->right = node;
      node->left = l_r;
      __mf_treerot_left ++;
    }
  else if (UNLIKELY ((node->right && node->right->data.liveness > node->data.liveness) &&
		     ((!node->left || (node->left && 
				       node->right->data.liveness > 
				       node->left->data.liveness)))))
    {
      __mf_object_tree_t *r = node->right;
      __mf_object_tree_t *r_l = r->left;

      *nodep = r;
      r->left = node;
      node->right = r_l;
      __mf_treerot_right ++;
    }

  return count;
}


unsigned
__mf_find_objects (uintptr_t ptr_low, uintptr_t ptr_high,
		   __mf_object_tree_t **objs, unsigned max_objs)
{
  if (UNLIKELY(__mf_opts.internal_checking))
    __mf_validate_objects ();

  return __mf_find_objects_rec (ptr_low, ptr_high, & __mf_object_root, objs, max_objs);
}

/* __mf_link_object */

static void
__mf_link_object2 (__mf_object_tree_t *ptr, __mf_object_tree_t **link)
{
  __mf_object_tree_t *node = *link;

  assert (ptr != NULL);
  if (UNLIKELY(node == NULL))
    {
      *link = ptr;
      return;
    }

  if (ptr->data.high < node->data.low)
    return __mf_link_object2 (ptr, & node->left);
  else if (ptr->data.low > node->data.high)
    return __mf_link_object2 (ptr, & node->right);
  else
    abort (); /* XXX: duplicate object */
}


void
__mf_link_object (__mf_object_tree_t *ptr)
{
  if (UNLIKELY(__mf_opts.internal_checking))
    __mf_validate_objects ();

  return __mf_link_object2 (ptr, & __mf_object_root);
}

/* __mf_unlink_object */

static void
__mf_unlink_object2 (__mf_object_tree_t *ptr, __mf_object_tree_t **link)
{
  __mf_object_tree_t *node = *link;
  
  assert (ptr != NULL);
  if (UNLIKELY(node == ptr))
    {
      static unsigned promote_left_p = 0;
      promote_left_p = 1 - promote_left_p;

      /* Alternate promoting the left & right subtrees. */
      if (promote_left_p)
	{
	  *link = ptr->left;
	  if (ptr->right != NULL)
	    __mf_link_object2 (ptr->right, link);
	}
      else
	{
	  *link = ptr->right;
	  if (ptr->left != NULL)
	    __mf_link_object2 (ptr->left, link);
	}

      return;
    }

  if (ptr->data.high < node->data.low)
    return __mf_unlink_object2 (ptr, & node->left);
  else if (ptr->data.low > node->data.high)
    return __mf_unlink_object2 (ptr, & node->right);
  else
    abort (); /* XXX: missing object; should fail more gracefully. */
}

static void
__mf_unlink_object (__mf_object_tree_t *node)
{
  __mf_unlink_object2 (node, & __mf_object_root);
}

/* __mf_find_dead_objects */

/* Find overlapping dead objecs between [low,high].  Return up to
   max_objs of their pointers in objs[].  Return total count of
   overlaps (may exceed max_objs).  */

static unsigned
__mf_find_dead_objects (uintptr_t low, uintptr_t high,
			__mf_object_tree_t **objs, unsigned max_objs)
{
  if (__mf_opts.persistent_count > 0)
    {
      unsigned count = 0;
      unsigned recollection = 0;
      unsigned row = 0;
      
      assert (low <= high);
      assert (max_objs == 0 || objs != NULL);
      
      /* Widen the search from the most recent plots in each row, looking
	 backward in time.  */
      recollection = 0;
      while (recollection < __mf_opts.persistent_count)
	{
	  count = 0;
	  
	  for (row = 0; row <= __MF_TYPE_MAX_CEM; row ++)
	    {
	      unsigned plot;
	      unsigned i;
	      
	      plot = __mf_object_dead_head [row];
	      for (i = 0; i <= recollection; i ++)
		{
		  __mf_object_tree_t *obj;
		  
		  /* Look backward through row: it's a circular buffer.  */
		  if (plot > 0) plot --;
		  else plot = __mf_opts.persistent_count - 1;
		  
		  obj = __mf_object_cemetary [row][plot];
		  if (obj && obj->data.low <= high && obj->data.high >= low)
		    {
		      /* Found an overlapping dead object!  */
		      if (count < max_objs)
			objs [count] = obj;
		      count ++;
		    }
		}
	    }
	  
	  if (count)
	    break;
	  
	  /* Look farther back in time.  */
	  recollection = (recollection * 2) + 1;
	}
      
      return count;
    } else {
      return 0;
    }
}

/* __mf_describe_object */

static void
__mf_describe_object (__mf_object_t *obj)
{
  static unsigned epoch = 0;
  if (obj == NULL)
    {
      epoch ++;
      return;
    }

  if (__mf_opts.abbreviate && obj->description_epoch == epoch)
    {
      fprintf (stderr,
	       "mudflap object %08lx: name=`%s'\n",
	       (uintptr_t) obj, (obj->name ? obj->name : ""));
      return;
    }
  else
    obj->description_epoch = epoch;

  fprintf (stderr,
	   "mudflap object %08lx: name=`%s'\n"
	   "bounds=[%08lx,%08lx] size=%lu area=%s check=%ur/%uw liveness=%u watching=%d\n"
	   "alloc time=%lu.%06lu pc=%08lx\n",
	   (uintptr_t) obj, (obj->name ? obj->name : ""), 
	   obj->low, obj->high, (obj->high - obj->low + 1),
	   (obj->type == __MF_TYPE_NOACCESS ? "no-access" :
	    obj->type == __MF_TYPE_HEAP ? "heap" :
	    obj->type == __MF_TYPE_HEAP_I ? "heap-init" :
	    obj->type == __MF_TYPE_STACK ? "stack" :
	    obj->type == __MF_TYPE_STATIC ? "static" :
	    obj->type == __MF_TYPE_GUESS ? "guess" :
	    "unknown"),
	   obj->read_count, obj->write_count, obj->liveness, obj->watching_p,
	   obj->alloc_time.tv_sec, obj->alloc_time.tv_usec, obj->alloc_pc);

  if (__mf_opts.backtrace > 0)
  {
    unsigned i;
    for (i=0; i<obj->alloc_backtrace_size; i++)
      fprintf (stderr, "      %s\n", obj->alloc_backtrace[i]);
  }

  if (__mf_opts.persistent_count > 0)
    {
      if (obj->deallocated_p)
	{
	  fprintf (stderr, "dealloc time=%lu.%06lu pc=%08lx\n",
		   obj->dealloc_time.tv_sec, obj->dealloc_time.tv_usec, obj->dealloc_pc);

	  if (__mf_opts.backtrace > 0)
	  {
	    unsigned i;
	    for (i=0; i<obj->dealloc_backtrace_size; i++)
	      fprintf (stderr, "      %s\n", obj->dealloc_backtrace[i]);
	  }
	}
    }
}

static unsigned
__mf_report_leaks (__mf_object_tree_t *node)
{
  static unsigned count = 0;  /* Shared amongst recursive calls.  */
  /* XXX: when to reset?  */

  if (node == NULL) return count;

  /* Inorder traversal. */
  __mf_report_leaks (node->left);
  if (node->data.type == __MF_TYPE_HEAP
      || node->data.type == __MF_TYPE_HEAP_I)
    {
      count ++;
      fprintf (stderr, "Leaked object %u:\n", count);
      __mf_describe_object (& node->data);
    }
  __mf_report_leaks (node->right);

  return count;
}

/* ------------------------------------------------------------------------ */
/* __mf_report */

void
__mf_report ()
{
  /* if (UNLIKELY (__mf_state == active)) return; */

  if (__mf_opts.collect_stats)
    {
      fprintf (stderr,
	       "*******\n"
	       "mudflap stats:\n"
	       "calls to __mf_check: %lu rot: %lu/%lu\n"
	       "         __mf_register: %lu [%luB, %luB, %luB, %luB, %luB]\n"
	       "         __mf_unregister: %lu [%luB]\n"
	       "         __mf_violation: [%lu, %lu, %lu, %lu, %lu]\n",
	       __mf_count_check, __mf_treerot_left, __mf_treerot_right,
	       __mf_count_register,
	       __mf_total_register_size[0], __mf_total_register_size[1],
	       __mf_total_register_size[2], __mf_total_register_size[3],
	       __mf_total_register_size[4], /* XXX */
	       __mf_count_unregister, __mf_total_unregister_size,
	       __mf_count_violation[0], __mf_count_violation[1],
	       __mf_count_violation[2], __mf_count_violation[3],
	       __mf_count_violation[4]);

      /* Lookup cache stats.  */
      {
	unsigned i;
	unsigned max_reuse = 0;
	unsigned num_used = 0;
	unsigned num_unused = 0;

	for (i = 0; i < LOOKUP_CACHE_SIZE; i++)
	  {
	    if (__mf_lookup_cache_reusecount[i])
	      num_used ++;
	    else
	      num_unused ++;
	    if (max_reuse < __mf_lookup_cache_reusecount[i])
	      max_reuse = __mf_lookup_cache_reusecount[i];
	  }
	fprintf (stderr, "lookup cache slots used: %u  unused: %u  peak-reuse: %u\n",
		 num_used, num_unused, max_reuse);
      }

      {
	unsigned live_count;
	live_count = __mf_find_objects (MINPTR, MAXPTR, NULL, 0);
	fprintf (stderr, "number of live objects: %u\n", live_count);
      }

      if (__mf_opts.persistent_count > 0)
	{
	  unsigned dead_count = 0;
	  unsigned row, plot;
	  for (row = 0; row <= __MF_TYPE_MAX_CEM; row ++)
	    for (plot = 0 ; plot < __mf_opts.persistent_count; plot ++)
	      if (__mf_object_cemetary [row][plot] != 0)
		dead_count ++;
	  fprintf (stderr, "          zombie objects: %u\n", dead_count);
	}
    }
  if (__mf_opts.print_leaks && (__mf_opts.mudflap_mode == mode_check))
    {
      unsigned l;
      DECLARE (void *, alloca, size_t n);

      /* Free up any remaining alloca()'d blocks.  */
      (void) CALL_WRAP (alloca, 0); /* XXX: doesn't work in shared mode. */
      __mf_describe_object (NULL); /* Reset description epoch.  */
      l = __mf_report_leaks (__mf_object_root);
      fprintf (stderr, "number of leaked objects: %u\n", l);
    }
}

/* __mf_backtrace */

size_t
__mf_backtrace (char ***symbols, void *guess_pc, unsigned guess_omit_levels)
{
  void ** pc_array;
  unsigned pc_array_size = __mf_opts.backtrace + guess_omit_levels;
  unsigned remaining_size;
  unsigned omitted_size = 0;
  unsigned i;
  DECLARE (void, free, void *ptr);
  DECLARE (void *, calloc, size_t c, size_t n);
  DECLARE (void *, malloc, size_t n);

  pc_array = CALL_REAL (calloc, pc_array_size, sizeof (void *));
#ifdef HAVE_BACKTRACE
  pc_array_size = backtrace (pc_array, pc_array_size);
#else
#define FETCH(n) do { if (pc_array_size >= n) { \
                 pc_array[n] = __builtin_return_address(n); \
                 if (pc_array[n] == 0) pc_array_size = n; } } while (0)

  /* Unroll some calls __builtin_return_address because this function
     only takes a literal integer parameter.  */
  FETCH (0);
#if 0
  /* XXX: __builtin_return_address sometimes crashes (!) on >0 arguments,
     rather than simply returning 0.  :-(  */
  FETCH (1);
  FETCH (2);
  FETCH (3);
  FETCH (4);
  FETCH (5);
  FETCH (6);
  FETCH (7);
  FETCH (8);
  if (pc_array_size > 8) pc_array_size = 9;
#else
  if (pc_array_size > 0) pc_array_size = 1;
#endif

#undef FETCH
#endif

  /* We want to trim the first few levels of the stack traceback,
     since they contain libmudflap wrappers and junk.  If pc_array[]
     ends up containing a non-NULL guess_pc, then trim everything
     before that.  Otherwise, omit the first guess_omit_levels
     entries. */
  
  if (guess_pc != NULL)
    for (i=0; i<pc_array_size; i++)
      if (pc_array [i] == guess_pc)
	omitted_size = i;

  if (omitted_size == 0) /* No match? */
    if (pc_array_size > guess_omit_levels)
      omitted_size = guess_omit_levels;

  remaining_size = pc_array_size - omitted_size;

#ifdef HAVE_BACKTRACE_SYMBOLS
  *symbols = backtrace_symbols (pc_array + omitted_size, remaining_size);
#else
  {
    /* Let's construct a buffer by hand.  It will have <remaining_size>
       char*'s at the front, pointing at individual strings immediately
       afterwards.  */
    void *buffer;
    char *chars;
    char **pointers;
    enum { perline = 30 };
    buffer = CALL_REAL (malloc, remaining_size * (perline + sizeof(char *)));
    pointers = (char **) buffer;
    chars = (char *)buffer + (remaining_size * sizeof (char *));
    for (i = 0; i < remaining_size; i++)
      {
	pointers[i] = chars;
	sprintf (chars, "[0x%08lx]", pc_array [omitted_size + i]);
	chars = chars + perline;
      }
    *symbols = pointers;
  }
#endif
  CALL_REAL (free, pc_array);

  return remaining_size;
}

/* ------------------------------------------------------------------------ */
/* __mf_violation */

void
__mf_violation (void *ptr, size_t sz, uintptr_t pc, 
		const char *location, int type)
{
  char buf [128];
  static unsigned violation_number;
  DECLARE(void, free, void *ptr);
  BEGIN_RECURSION_PROTECT;

  TRACE ("mf: violation pc=%08lx location=%s type=%d ptr=%08lx size=%lu\n", pc, 
	 (location != NULL ? location : ""), type, ptr, sz);

  if (__mf_opts.collect_stats)
    __mf_count_violation [(type < 0) ? 0 :
			  (type > __MF_VIOL_WATCH) ? 0 :
			  type] ++;

  /* Print out a basic warning message.  */
  if (__mf_opts.verbose_violations)
  {
    unsigned dead_p;
    unsigned num_helpful = 0;
    struct timeval now;
    gettimeofday (& now, NULL);

    violation_number ++;
    fprintf (stderr,
	     "*******\n"
	     "mudflap violation %u (%s): time=%lu.%06lu "
	     "ptr=%08lx size=%lu pc=%08lx%s%s%s\n", 
	     violation_number,
	     ((type == __MF_VIOL_READ) ? "check/read" :
	      (type == __MF_VIOL_WRITE) ? "check/write" :
	      (type == __MF_VIOL_REGISTER) ? "register" :
	      (type == __MF_VIOL_UNREGISTER) ? "unregister" :
	      (type == __MF_VIOL_WATCH) ? "watch" : "unknown"),
	     now.tv_sec, now.tv_usec, 
	     ptr, sz, pc,
	     (location != NULL ? " location=`" : ""),
	     (location != NULL ? location : ""),
	     (location != NULL ? "'" : ""));

    if (__mf_opts.backtrace > 0)
      {
	char ** symbols;
	unsigned i, num;
	
	num = __mf_backtrace (& symbols, (void *) pc, 2);
	/* Note: backtrace_symbols calls malloc().  But since we're in
	   __mf_violation and presumably __mf_check, it'll detect
	   recursion, and not put the new string into the database.  */
	
	for (i=0; i<num; i++)
	  fprintf (stderr, "      %s\n", symbols[i]);
	
	/* Calling free() here would trigger a violation.  */
	CALL_REAL(free, symbols);
      }
    
    
    /* Look for nearby objects.  For this, we start with s_low/s_high
       pointing to the given area, looking for overlapping objects.
       If none show up, widen the search area and keep looking. */
    
    if (sz == 0) sz = 1;
    
    for (dead_p = 0; dead_p <= 1; dead_p ++) /* for dead_p in 0 1 */
      {
	enum {max_objs = 3}; /* magic */
	__mf_object_tree_t *objs[max_objs];
	unsigned num_objs = 0;
	uintptr_t s_low, s_high;
	unsigned tries = 0;
	unsigned i;
	
	s_low = (uintptr_t) ptr;
	s_high = CLAMPSZ (ptr, sz);

	while (tries < 16) /* magic */
	  {
	    if (dead_p)
	      num_objs = __mf_find_dead_objects (s_low, s_high, objs, max_objs);
	    else
	      num_objs = __mf_find_objects (s_low, s_high, objs, max_objs);

	    if (num_objs) /* good enough */
	      break;

	    tries ++;

	    /* XXX: tune this search strategy.  It's too dependent on
	     sz, which can vary from 1 to very big (when array index
	     checking) numbers. */
	    s_low = CLAMPSUB (s_low, (sz * tries * tries));
	    s_high = CLAMPADD (s_high, (sz * tries * tries));
	  }

	for (i = 0; i < min (num_objs, max_objs); i++)
	  {
	    __mf_object_t *obj = & objs[i]->data;
	    uintptr_t low = (uintptr_t) ptr;
	    uintptr_t high = CLAMPSZ (ptr, sz);
	    unsigned before1 = (low < obj->low) ? obj->low - low : 0;
	    unsigned after1 = (low > obj->high) ? low - obj->high : 0;
	    unsigned into1 = (high >= obj->low && low <= obj->high) ? low - obj->low : 0;
	    unsigned before2 = (high < obj->low) ? obj->low - high : 0;
	    unsigned after2 = (high > obj->high) ? high - obj->high : 0;
	    unsigned into2 = (high >= obj->low && low <= obj->high) ? high - obj->low : 0;

	    fprintf (stderr, "Nearby object %u: checked region begins %uB %s and ends %uB %s\n",
		     num_helpful + i + 1,
		     (before1 ? before1 : after1 ? after1 : into1),
		     (before1 ? "before" : after1 ? "after" : "into"),
		     (before2 ? before2 : after2 ? after2 : into2),
		     (before2 ? "before" : after2 ? "after" : "into"));
	    __mf_describe_object (obj);
	  }
	num_helpful += num_objs;
      }

    fprintf (stderr, "number of nearby objects: %u\n", num_helpful);
  }

  /* How to finally handle this violation?  */
  switch (__mf_opts.violation_mode)
    {
    case viol_nop:
      break;
    case viol_segv:
      kill (getpid(), SIGSEGV);
      break;
    case viol_abort:
      abort ();
      break;
    case viol_gdb:
      snprintf (buf, 128, "gdb --pid=%d", getpid ());
      system (buf);
      /* XXX: should probably fork() && sleep(GDB_WAIT_PARAMETER)
      instead, and let the forked child execlp() gdb.  That way, this
      subject process can be resumed under the supervision of gdb.
      This can't happen now, since system() only returns when gdb
      dies.  In that case, we need to beware of starting a second
      concurrent gdb child upon the next violation.  (But if the first
      gdb dies, then starting a new one is appropriate.)  */
      break;
    }
  
  END_RECURSION_PROTECT;
}

/* ------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------ */
/* __mf_check */

unsigned __mf_watch (void *ptr, size_t sz)
{
  return __mf_watch_or_not (ptr, sz, 1);
}

unsigned __mf_unwatch (void *ptr, size_t sz)
{
  return __mf_watch_or_not (ptr, sz, 0);
}


static unsigned
__mf_watch_or_not (void *ptr, size_t sz, char flag)
{
  uintptr_t ptr_high = CLAMPSZ (ptr, sz);
  uintptr_t ptr_low = (uintptr_t) ptr;
  unsigned count = 0;

  TRACE ("mf: %s ptr=%08lx size=%lu",
	 (flag ? "watch" : "unwatch"), ptr, sz);
  
  switch (__mf_opts.mudflap_mode)
    {
    case mode_nop:
    case mode_populate:
    case mode_violate:
      count = 0;
      break;

    case mode_check:
      {
	__mf_object_tree_t **all_ovr_objs;
	unsigned obj_count;
	unsigned n;
	DECLARE (void *, malloc, size_t c);
	DECLARE (void, free, void *p);

	obj_count = __mf_find_objects (ptr_low, ptr_high, NULL, 0);
	VERBOSE_TRACE (" %u:", obj_count);

	all_ovr_objs = CALL_REAL (malloc, (sizeof (__mf_object_tree_t *) *
					   obj_count));
	if (all_ovr_objs == NULL) abort ();
	n = __mf_find_objects (ptr_low, ptr_high, all_ovr_objs, obj_count);
	assert (n == obj_count);

	for (n = 0; n < obj_count; n ++)
	  {
	    __mf_object_t *obj = & (all_ovr_objs[n]->data);

	    VERBOSE_TRACE (" [%08lx]", (uintptr_t) obj);
	    if (obj->watching_p != flag)
	      {
		obj->watching_p = flag;
		count ++;

		/* Remove object from cache, to ensure next access
		   goes through __mf_check().  */
	        if (flag)
		  __mf_uncache_object (obj);
	      }
	  }
	CALL_REAL (free, all_ovr_objs);
      }
      break;
    }

  TRACE (" hits=%u\n", count);

  return count;
}
