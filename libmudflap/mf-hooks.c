/* {{{ Copyright */
/* Mudflap: narrow-pointer bounds-checking by tree rewriting.
   Copyright (C) 2002 Free Software Foundation, Inc.
   Contributed by Frank Ch. Eigler <fche@redhat.com>
   and Graydon Hoare <graydon@redhat.com>

This file is part of GCC.
XXX: libgcc license?
*/

/* }}} */

#include <string.h>
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


/* XXX: if -DPIC, use dlsym-style wrapping instead */

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
  /* Use a circular queue to delay some number (__mf_opts.free_queue_length) of free()s.  */
  static void *free_queue [__MF_FREEQ_MAX];
  static unsigned free_ptr;

  extern void * __real_free (void *);

  if (buf == NULL)
    return;

  __mf_unregister ((uintptr_t) buf, 0);
      
  if (__mf_opts.free_queue_length > 0) 
    {
      
      if (free_queue [free_ptr] != NULL)
	__wrap_free (free_queue [free_ptr]);
      free_queue [free_ptr] = buf;
      free_ptr = (free_ptr == (__mf_opts.free_queue_length-1) ? 0 : free_ptr + 1);
    } 
  else 
    {
      __real_free (buf);
    }
}


/* }}} */
/* {{{ str*,mem*,b* */

#define UNLIKELY(e) (__builtin_expect (!!(e), 0))
#define LIKELY(e) (__builtin_expect (!!(e), 1))

#define MF_VALIDATE_EXTENT(value,size)                        \
 {                                                            \
  struct __mf_cache * elem =                                  \
    & __mf_lookup_cache [((unsigned)value >> __mf_lc_shift) & \
			 __mf_lc_mask];                       \
  if (UNLIKELY ((elem->low > (unsigned)value) ||              \
		(elem->high < (unsigned)value+size)))         \
    __mf_check ((unsigned)value, size);                       \
 }


void *
__wrap_memcpy (void *dest, const void *src, size_t n)
{
  extern void *__real_memcpy (void *dest, const void *src, size_t n);
  MF_VALIDATE_EXTENT(src, n);
  MF_VALIDATE_EXTENT(dest, n);
  return __real_memcpy (dest, src, n);
}



void *
__wrap_memmove (void *dest, const void *src, size_t n)
{
  extern void *__real_memmove (void *dest, const void *src, size_t n);
  MF_VALIDATE_EXTENT(src, n);
  MF_VALIDATE_EXTENT(dest, n);
  return __real_memmove (dest, src, n);
}


void *
__wrap_memset (void *s, int c, size_t n) 
{
  extern void *__real_memset (void *s, int c, size_t n);
  MF_VALIDATE_EXTENT(s, n);
  return __real_memset (s, c, n);
}

int
__wrap_memcmp (const void *s1, const void *s2, size_t n)
{
  extern int __real_memcmp (const void *s1, const void *s2, size_t n);
  MF_VALIDATE_EXTENT(s1, n);
  MF_VALIDATE_EXTENT(s2, n);
  return __real_memcmp (s1, s2, n);
}

void *
__wrap_memchr (const void *s, int c, size_t n)
{
  extern void *__real_memchr (const void *s, int c, size_t n);
  MF_VALIDATE_EXTENT(s, n);
  return __real_memchr (s, c, n);
}

void *
__wrap_memrchr (const void *s, int c, size_t n)
{
  extern void *__real_memrchr (const void *s, int c, size_t n);
  MF_VALIDATE_EXTENT(s, n);
  return __real_memrchr (s, c, n);
}


char *
__wrap_strcpy (char *dest, const char *src)
{
  extern char *__real_strcpy (char *dest, const char *src);
  int n = strlen (src);

  /* nb: just because strlen(src) == n doesn't mean (src + n) or (src + n +
     1) are valid pointers. the allocated object might have size < n.
     check anyways. */

  MF_VALIDATE_EXTENT(src, (n + 1)); 
  MF_VALIDATE_EXTENT(dest, (n + 1));
  return __real_strcpy (dest, src);
}

char *
__wrap_strncpy (char *dest, const char *src, size_t n)
{
  extern char *__real_strncpy (char *dest, const char *src, size_t n);
  int len = strnlen (src, n);
  MF_VALIDATE_EXTENT(src, len);
  MF_VALIDATE_EXTENT(dest, (len + 1));
  return __real_strncpy (dest, src, n);
}

char *
__wrap_strcat (char *dest, const char *src)
{
  extern char *__real_strcat (char *dest, const char *src);
  int dest_sz = strlen (dest);
  int src_sz = strlen (src);  
  MF_VALIDATE_EXTENT(src, (src_sz + 1));
  MF_VALIDATE_EXTENT(dest, (dest_sz + src_sz + 1));
  return __real_strcat (dest, src);
}

char *
__wrap_strncat (char *dest, const char *src, size_t n)
{
  extern char *__real_strncat (char *dest, const char *src, size_t n);

  /* nb: validating the extents (s,n) might be a mistake for two reasons.
     
  (1) the string s might be shorter than n chars, and n is just a 
  poor choice by the programmer. this is not a "true" error in the
  sense that the call to strncat would still be ok.
  
  (2) we could try to compensate for case (1) by calling strlen(s) and
  using that as a bound for the extent to verify, but strlen might fall off
  the end of a non-terminated string, leading to a false positive.
  
  so we will call strnlen(s,n) and use that as a bound.

  if strnlen returns a length beyond the end of the registered extent
  associated with s, there is an error: the programmer's estimate for n is
  too large _AND_ the string s is unterminated, in which case they'd be
  about to touch memory they don't own while calling strncat.

  this same logic applies to further uses of strnlen later down in this
  file. */

  int src_sz = strnlen (src, n);
  int dest_sz = strnlen (dest, n);
  MF_VALIDATE_EXTENT(src, src_sz);
  MF_VALIDATE_EXTENT(dest, (dest_sz + src_sz + 1));
  return __real_strncat (dest, src, n);
}

int
__wrap_strcmp (const char *s1, const char *s2)
{
  extern int __real_strcmp (const char *s1, const char *s2);
  int s1_sz = strlen (s1);
  int s2_sz = strlen (s2);  
  MF_VALIDATE_EXTENT(s1, s1_sz);
  MF_VALIDATE_EXTENT(s2, s2_sz);
  return __real_strcmp (s1, s2);
}

int
__wrap_strcasecmp (const char *s1, const char *s2)
{
  extern int __real_strcasecmp (const char *s1, const char *s2);
  int s1_sz = strlen (s1);
  int s2_sz = strlen (s2);  
  MF_VALIDATE_EXTENT(s1, s1_sz);
  MF_VALIDATE_EXTENT(s2, s2_sz);
  return __real_strcasecmp (s1, s2);
}

int
__wrap_strncmp (const char *s1, const char *s2, size_t n)
{
  extern int __real_strncmp (const char *s1, const char *s2, size_t n);
  int s1_sz = strnlen (s1, n);
  int s2_sz = strnlen (s2, n);
  MF_VALIDATE_EXTENT(s1, s1_sz);
  MF_VALIDATE_EXTENT(s2, s2_sz);
  return __real_strncmp (s1, s2, n);
}

int
__wrap_strncasecmp (const char *s1, const char *s2, size_t n)
{
  extern int __real_strncasecmp (const char *s1, const char *s2, size_t n);
  int s1_sz = strnlen (s1, n);
  int s2_sz = strnlen (s2, n);
  MF_VALIDATE_EXTENT(s1, s1_sz);
  MF_VALIDATE_EXTENT(s2, s2_sz);
  return __real_strncasecmp (s1, s2, n);
}

char *
__wrap_strdup (const char *s)
{
  extern char *__real_strdup (const char *s);
  int n = strlen (s);
  char *result;
  MF_VALIDATE_EXTENT(s, n);
  result = __real_strdup (s);
  __mf_register ((uintptr_t) result, n, __MF_TYPE_HEAP, "strdup region");
  return result;
}

char *
__wrap_strndup (const char *s, size_t n)
{
  extern char *__real_strndup (const char *s, size_t n);
  char *result;
  int sz = strnlen (s, n);
  MF_VALIDATE_EXTENT(s, sz);
  result = __real_strndup (s, n);
  __mf_register ((uintptr_t) result, sz, __MF_TYPE_HEAP, "strndup region");
  return result;
}


char *
__wrap_strchr (const char *s, int c)
{
  extern char *__real_strchr (const char *s, int c);
  int n = strlen (s);
  MF_VALIDATE_EXTENT(s, n);
  return __real_strchr (s, c);
}

char *
__wrap_strrchr (const char *s, int c)
{
  extern char *__real_strrchr (const char *s, int c);
  int n = strlen (s);
  MF_VALIDATE_EXTENT(s, n);
  return __real_strrchr (s, c);
}


char *
__wrap_strstr (const char *haystack, const char *needle)
{
  extern char *__real_strstr (const char *haystack, const char *needle);
  int haystack_sz = strlen (haystack);
  int needle_sz = strlen (needle);
  MF_VALIDATE_EXTENT(haystack, haystack_sz);
  MF_VALIDATE_EXTENT(needle, needle_sz);
  return __real_strstr (haystack, needle);
}


void *
__wrap_memmem (const void *haystack, size_t haystacklen,
	       const void *needle, size_t needlelen)
{
  extern void *__real_memmem (const void *haystack, size_t haystacklen,
			      const void *needle, size_t needlelen);
  MF_VALIDATE_EXTENT(haystack, haystacklen);
  MF_VALIDATE_EXTENT(needle, needlelen);
  return __real_memmem (haystack, haystacklen, needle, needlelen);
}

int
__wrap_strlen (const char *s)
{
  extern int __real_strlen (const char *s);
  int n = __real_strlen (s);
  MF_VALIDATE_EXTENT(s, n);
  return n;
}

int
__wrap_strnlen (const char *s, size_t n)
{
  extern int __real_strnlen (const char *s, size_t n);
  int m = __real_strnlen (s, n);
  MF_VALIDATE_EXTENT(s, m);
  return m;
}

void
__wrap_bzero (void *s, size_t n)
{
  extern void __real_bzero (void *s, size_t n);
  MF_VALIDATE_EXTENT(s, n);
  return __real_bzero (s, n);
}

void
__wrap_bcopy (const void *src, void *dest, size_t n)
{
  extern void __real_bcopy (const void *src, void *dest, size_t n);
  MF_VALIDATE_EXTENT(src, n);
  MF_VALIDATE_EXTENT(dest, n);
  return __real_bcopy (src, dest, n);
}

int
__wrap_bcmp (const void *s1, const void *s2, size_t n)
{
  extern int __real_bcmp (const void *s1, const void *s2, size_t n);
  MF_VALIDATE_EXTENT(s1, n);
  MF_VALIDATE_EXTENT(s2, n);
  return __real_bcmp (s1, s2, n);
}

char *
__wrap_index (const char *s, int c)
{
  extern char *__real_index (const char *s, int c);
  int n = strlen (s);
  MF_VALIDATE_EXTENT(s, n);
  return __real_index (s, c);
}

char *
__wrap_rindex (const char *s, int c)
{
  extern char *__real_rindex (const char *s, int c);
  int n = strlen (s);
  MF_VALIDATE_EXTENT(s, n);
  return __real_rindex (s, c);
}

/* 

general form: 

void
__wrap_ ()
{
  extern void __real_ ();
  MF_VALIDATE_EXTENT();
  return __real_ ();
}

*/


/* }}} */
/* {{{ *printf,*scanf */



/* }}} */


/* ------------------------------------------------------------------------ */
