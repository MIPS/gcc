#ifndef __MF_RUNTIME_H
#define __MF_RUNTIME_H


/* ------------------------------------------------------------------------ */
/* Public libmudflap declarations */

#ifndef HAVE_UINTPTR_T
#define HAVE_UINTPTR_T
typedef unsigned long uintptr_t;
#endif

struct __mf_cache { uintptr_t low; uintptr_t high; };
extern struct __mf_cache __mf_lookup_cache [];
extern uintptr_t __mf_lc_mask;
extern unsigned char __mf_lc_shift;
#define __MF_CACHE_INDEX(ptr) ((((uintptr_t) (ptr)) >> __mf_lc_shift) & __mf_lc_mask)
#define __MF_CACHE_MISS_P(ptr,sz) ({ \
             struct __mf_cache *elem = & __mf_lookup_cache[__MF_CACHE_INDEX((ptr))]; \
             ((elem->low > (uintptr_t) (ptr)) ||                  \
	      (elem->high < ((uintptr_t) (ptr) + (uintptr_t) ((sz) - 1)))); })
/* XXX: the above should use CLAMPSZ () */
extern void __mf_check (uintptr_t ptr, uintptr_t sz, const char *location);


/* ------------------------------------------------------------------------ */
/* Internal libmudflap declarations */
/* XXX: should be in a separate file.  */

typedef enum {inactive, starting, active, reentrant} mf_state;
extern mf_state __mf_state;
#define UNLIKELY(e) (__builtin_expect (!!(e), 0))
#define LIKELY(e) (__builtin_expect (!!(e), 1))

extern void __mf_init_heuristics ();

#define __MF_VIOL_UNKNOWN 0
#define __MF_VIOL_CHECK 1
#define __MF_VIOL_REGISTER 2
#define __MF_VIOL_UNREGISTER 3
extern void __mf_violation (uintptr_t ptr, uintptr_t sz, 
			    uintptr_t pc, const char *location, 
			    int type);
#define __MF_TYPE_UNKNOWN 0
#define __MF_TYPE_HEAP 1
#define __MF_TYPE_STACK 2
#define __MF_TYPE_STATIC 3
#define __MF_TYPE_GUESS 4
extern void __mf_register (uintptr_t ptr, uintptr_t sz, int type, const char *name);
extern void __mf_unregister (uintptr_t ptr, uintptr_t sz);
extern void __mf_report ();
extern size_t __mf_backtrace (char ***, void *, unsigned);

#define __MF_PERSIST_MAX 256
#define __MF_FREEQ_MAX 256

#define TRACE(...)                         \
  if (UNLIKELY (__mf_opts.trace_mf_calls)) \
      fprintf (stderr, __VA_ARGS__);

#define TRACE_IN TRACE("mf: enter %s\n", __FUNCTION__)
#define TRACE_OUT TRACE("mf: exit %s\n", __FUNCTION__)

struct __mf_options
{
  /* Emit a trace message for each call. */
  int trace_mf_calls;

  /* Collect and emit statistics. */
  int collect_stats;

  /* Execute internal checking code */
  int internal_checking;

  /* Print list of leaked heap objects on shutdown. */
  int print_leaks;       

  /* Print verbose description of violations. */
  int verbose_violations;

  /* Perform occasional tree-rotations to optimize lookups. */
  int optimize_object_tree;

  /* Support multiple threads. */
  int multi_threaded;

  /* use __libc_stack_end heuristic */
  int stack_bound;
  
  /* Maintain a queue of this many deferred free()s, 
     to trap use of freed memory. */
  int free_queue_length;

  /* Maintain a history of this many past unregistered objects. */
  int persistent_count;

  /* Pad allocated extents by this many bytes on either side. */
  int crumple_zone;

  /* Maintain this many stack frames for contexts. */
  int backtrace;

  /* Major operation mode */

  enum
  {
    mode_nop,        /* mudflaps do nothing */
    mode_populate,   /* mudflaps populate tree but do not check for violations */
    mode_check,      /* mudflaps populate and check for violations (normal) */
    mode_violate     /* mudflaps trigger a violation on every call (diagnostic) */
  }
  mudflap_mode;


  /* How to handle a violation. */

  enum
  {
    viol_nop,        /* Return control to application. */ 
    viol_segv,       /* Signal self with segv. */
    viol_abort,      /* Call abort (). */
    viol_gdb         /* Fork a debugger on self */
  }
  violation_mode;

  /* Violation heuristics selection. */
  int heur_proc_map;  /* Register /proc/self/map regions.  */
};

extern struct __mf_options __mf_opts;

#endif
