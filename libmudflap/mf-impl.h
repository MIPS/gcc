#ifndef __MF_IMPL_H
#define __MF_IMPL_H

/* 
   Implementation header for mudflap runtime library.
   
   Mudflap: narrow-pointer bounds-checking by tree rewriting.  
   Copyright (C) 2002, 2003 Free Software Foundation, Inc.  
   Contributed by Frank Ch. Eigler <fche@redhat.com> 
   and Graydon Hoare <graydon@redhat.com>
   
   This file is part of GCC.

*/

#ifdef _MUDFLAP
#error "Do not compile this file with -fmudflap!"
#endif


/* Address calculation macros.  */


/* XXX: these macros should be in an __MF*-like namespace. */

#define MINPTR ((uintptr_t) 0)
#define MAXPTR (~ (uintptr_t) 0)

/* Clamp the addition/subtraction of uintptr_t's to [MINPTR,MAXPTR] */
#define CLAMPSUB(ptr,offset) (((uintptr_t) ptr) >= (offset) ? ((uintptr_t) ptr)-((uintptr_t) offset) : MINPTR)
#define CLAMPADD(ptr,offset) (((uintptr_t) ptr) <= MAXPTR-(offset) ? ((uintptr_t) ptr)+((uintptr_t) offset) : MAXPTR)
#define CLAMPSZ(ptr,size) ((size) ? (((uintptr_t) ptr) <= MAXPTR-(size)+1 ? ((uintptr_t) ptr)+((uintptr_t) size) - 1 : MAXPTR) : ((uintptr_t) ptr))

#define __MF_CACHE_INDEX(ptr) ((((uintptr_t) (ptr)) >> __mf_lc_shift) & __mf_lc_mask)
#define __MF_CACHE_MISS_P(ptr,sz) ({ \
             struct __mf_cache *elem = & __mf_lookup_cache[__MF_CACHE_INDEX((ptr))]; \
             ((elem->low > (uintptr_t) (ptr)) ||                  \
	      (elem->high < (CLAMPADD((uintptr_t) (ptr), (uintptr_t) CLAMPSUB(sz,1) )))); })
/* XXX: the above should use CLAMPSZ () */



/* Private functions. */ 

extern void __mf_violation (void *ptr, size_t sz, 
			    uintptr_t pc, const char *location, 
			    int type);

extern size_t __mf_backtrace (char ***, void *, unsigned);
extern void __mf_resolve_dynamics ();
extern int __mf_heuristic_check (uintptr_t, uintptr_t);


/* ------------------------------------------------------------------------ */
/* Type definitions. */
/* ------------------------------------------------------------------------ */

/* The mf_state type codes describe recursion and initialization order. */

enum __mf_state
  {
    inactive, 
    starting, 
    active, 
    reentrant
  }; 

/* The __mf_options structure records optional or tunable aspects of the
 mudflap library's behavior. There is a single global instance of this
 structure which is populated from user input (in an environment variable)
 when the library initializes. */

struct __mf_options
{
  /* Emit a trace message for each call. */
  unsigned trace_mf_calls;

  /* Collect and emit statistics. */
  unsigned collect_stats;

  /* Execute internal checking code. */
  unsigned internal_checking;

  /* Age object liveness periodically. */
  unsigned tree_aging;

  /* Adapt the lookup cache to working set. */
  unsigned adapt_cache;

  /* Print list of leaked heap objects on shutdown. */
  unsigned print_leaks;       

  /* Detect reads of uninitialized objects. */
  unsigned check_initialization;       

  /* Print verbose description of violations. */
  unsigned verbose_violations;

  /* Abbreviate duplicate object descriptions.  */
  unsigned abbreviate;

  /* Emit internal tracing message. */
  unsigned verbose_trace;

  /* Support multiple threads. */
  unsigned multi_threaded;

  /* Wipe stack/heap objects upon unwind.  */
  unsigned wipe_stack;
  unsigned wipe_heap;

  /* Maintain a queue of this many deferred free()s, 
     to trap use of freed memory. */
  unsigned free_queue_length;

  /* Maintain a history of this many past unregistered objects. */
  unsigned persistent_count;

  /* Pad allocated extents by this many bytes on either side. */
  unsigned crumple_zone;

  /* Maintain this many stack frames for contexts. */
  unsigned backtrace;

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
  unsigned heur_stack_bound; /* allow current stack region */
  unsigned heur_proc_map;  /* allow & cache /proc/self/map regions.  */
  unsigned heur_start_end; /* allow _start .. _end */
  unsigned heur_argv_environ; /* allow & cache argv[] & environ */
};


#ifdef PIC

/* This is a table of dynamically resolved function pointers. */

struct __mf_dynamic 
{
  void * dyn_bcmp;
  void * dyn_bcopy;
  void * dyn_bzero;
  void * dyn_calloc;
  void * dyn_dlopen;
  void * dyn_free;
  void * dyn_index;
  void * dyn_malloc;
  void * dyn_memchr;
  void * dyn_memcmp;
  void * dyn_memcpy;
  void * dyn_memmem;
  void * dyn_memmove;
  void * dyn_memrchr;
  void * dyn_memset;
  void * dyn_mmap;
  void * dyn_munmap;
  void * dyn_realloc;
  void * dyn_rindex;
  void * dyn_strcasecmp;
  void * dyn_strcat;
  void * dyn_strchr;
  void * dyn_strcmp;
  void * dyn_strcpy;
  void * dyn_strdup;
  void * dyn_strlen;
  void * dyn_strncasecmp;
  void * dyn_strncat;
  void * dyn_strncmp;
  void * dyn_strncpy;
  void * dyn_strndup;
  void * dyn_strnlen;
  void * dyn_strrchr;
  void * dyn_strstr;
};

#endif /* PIC */

/* ------------------------------------------------------------------------ */
/* Private global variables. */
/* ------------------------------------------------------------------------ */

extern enum __mf_state __mf_state;
extern struct __mf_options __mf_opts;
#ifdef PIC
extern struct __mf_dynamic __mf_dynamic;
#endif 

/* ------------------------------------------------------------------------ */
/* Utility macros. */
/* ------------------------------------------------------------------------ */

#define UNLIKELY(e) (__builtin_expect (!!(e), 0))
#define LIKELY(e) (__builtin_expect (!!(e), 1))
#define STRINGIFY2(e) #e
#define STRINGIFY(e) STRINGIFY2(e)
#define VERBOSE_TRACE(...)                         \
  if (UNLIKELY (__mf_opts.verbose_trace)) \
      fprintf (stderr, __VA_ARGS__);
#define TRACE(...)                         \
  if (UNLIKELY (__mf_opts.trace_mf_calls)) \
      fprintf (stderr, __VA_ARGS__);

#define __MF_PERSIST_MAX 256
#define __MF_FREEQ_MAX 256

/* 
   Wrapping and redirection:

   Mudflap redirects a number of libc functions into itself, for "cheap"
   verification (eg. strcpy, bzero, memcpy) and also to register /
   unregister regions of memory as they are manipulated by the program
   (eg. malloc/free, mmap/munmap).

   There are two methods of wrapping. 

   (1) The static method involves a list of -wrap=foo flags being passed to
   the linker, which then links references to "foo" to the symbol
   "__wrap_foo", and links references to "__real_foo" to the symbol "foo".
   When compiled without -DPIC, libmudflap.a contains such __wrap_foo
   functions which delegate to __real_foo functions in libc to get their
   work done.

   (2) The dynamic method involves providing a definition of symbol foo in
   libmudflap.so and linking it earlier in the compiler command line,
   before libc.so. The function "foo" in libmudflap must then call
   dlsym(RTLD_NEXT, "foo") to acquire a pointer to the "real" libc foo, or
   at least the "next" foo in the dynamic link resolution order.

   We switch between these two techniques by the presence of the -DPIC
   #define passed in by libtool when building libmudflap.
*/


#ifdef PIC
#define __USE_GNU
#include <dlfcn.h>

#define WRAPPER(ret, fname, ...)                      \
ret __wrap_ ## fname (__VA_ARGS__)                    \
    __attribute__ (( alias  (#fname)  ));             \
ret __real_ ## fname (__VA_ARGS__)                    \
    __attribute__ (( alias  (#fname)  ));             \
ret fname (__VA_ARGS__)
#define DECLARE(ty, fname, ...)                       \
 typedef ty (*__mf_fn_ ## fname) (__VA_ARGS__)
#define CALL_REAL(fname, ...)                         \
  ({ if (UNLIKELY(!__mf_dynamic.dyn_ ## fname))       \
     __mf_resolve_dynamics ();                        \
  ((__mf_fn_ ## fname)(__mf_dynamic.dyn_ ## fname))   \
                      (__VA_ARGS__);})
#define CALL_WRAP(fname, ...)                         \
  (__wrap_ ## fname (__VA_ARGS__))

#else /* not PIC --> static library */

#define WRAPPER(ret, fname, ...)            \
ret __wrap_ ## fname (__VA_ARGS__)
#define DECLARE(ty, fname, ...)             \
 extern ty __real_ ## fname (__VA_ARGS__)
#define CALL_REAL(fname, ...)               \
 __real_ ## fname (__VA_ARGS__)
#define CALL_WRAP(fname, ...)               \
 __wrap_ ## fname (__VA_ARGS__)

#endif /* PIC */


/* previous incarnation of PIC strategy */

/* 
#define WRAPPER(fname) fname

#define DECLARE(ty, fname, ...)                  \
 typedef ty (*__mf_fn_ ## fname) (__VA_ARGS__);  \
 static __mf_fn_ ## fname __dynamic_ ## fname = NULL;

#define CALL_REAL(fname, ...)               \
({                                          \
 if (__dynamic_ ## fname == NULL)           \
 {                                          \
  enum __mf_state prev_state;               \
  prev_state = __mf_state;                  \
  __mf_state = active;                      \
  char *err = NULL;                         \
  __dynamic_ ## fname = (__mf_fn_ ## fname) dlsym (RTLD_NEXT, #fname);   \
  err = dlerror ();                         \
  if (err != NULL)                          \
  {                                         \
    fprintf (stderr, "mf: error dlsym(" STRINGIFY(fname) ") failed: %s\n", err); \
    abort ();                               \
  }                                         \
    else if (__dynamic_ ## fname == NULL)            \
  {                                         \
    fprintf (stderr, "mf: error dlsym(" STRINGIFY(fname) ") returned null\n"); \
    abort ();                               \
  }                                         \
  __mf_state = prev_state;                  \
 }                                          \
 (*__dynamic_ ## fname) (__VA_ARGS__);      \
})
 */


#endif /* __MF_IMPL_H */
