/* {{{ Copyright */
/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>

This file is part of GCC.
XXX: libgcc license?
*/

/* }}} */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <execinfo.h>
#include <assert.h>

#include "mf-runtime.h"


#ifdef _MUDFLAP
#error "Do not compile this file with -fmudflap!"
#endif


/* XXX: if -DSHARED, use dlsym-style wrapping instead */

/* {{{ malloc/free etc. */
void *
__wrap_malloc (size_t c)
{
  void *buf;
  extern void * __real_malloc (size_t);

  buf = (void *) __real_malloc (c);
  if (buf)
    __mf_register ((uintptr_t) buf, c, __MF_TYPE_HEAP, "malloc region");

  return buf;
}


void *
__wrap_calloc (size_t c, size_t n)
{
  void *buf;
  extern void * __real_calloc (size_t c, size_t);

  buf = (void *) __real_calloc (c, n);
  if (buf)
    __mf_register ((uintptr_t) buf, c*n, __MF_TYPE_HEAP, "calloc region");

  return buf;
}

void *
__wrap_realloc (void *buf, size_t c)
{
  void *buf2;
  extern void * __real_realloc (void *, size_t);

  buf2 = (void *) __real_realloc (buf, c);
  if (buf)
    __mf_unregister ((uintptr_t) buf, 0);
  if (buf2)
    __mf_register ((uintptr_t) buf2, c, __MF_TYPE_HEAP, "realloc region");

  return buf2;
}


void
__wrap_free (void *buf)
{
#if OPT_FREEQ
  /* Use a circular queue to delay some number (OPT_FREEQ) of free()s.  */
  static void *free_queue [OPT_FREEQ];
  static unsigned free_ptr;
#endif

  extern void * __real_free (void *);

  if (buf == NULL)
    return;

  __mf_unregister ((uintptr_t) buf, 0);
      
#if OPT_FREEQ
  if (free_queue [free_ptr] != NULL)
    __wrap_free (free_queue [free_ptr]);
  free_queue [free_ptr] = buf;
  free_ptr = (free_ptr == (OPT_FREEQ-1) ? 0 : free_ptr + 1);
#else
  __real_free (buf);
#endif
}


/* }}} */
/* {{{ str*,mem*,b* */



/* }}} */
/* {{{ *printf,*scanf */



/* }}} */

/* ------------------------------------------------------------------------ */
