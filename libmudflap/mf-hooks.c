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


#define UNLIKELY(e) (__builtin_expect (!!(e), 0))
#define LIKELY(e) (__builtin_expect (!!(e), 1))

#define STRINGIFY2(e) #e
#define STRINGIFY(e) STRINGIFY2(e)
#define MF_VALIDATE_EXTENT(value,size)                        \
 {                                                            \
  if (UNLIKELY (__MF_CACHE_MISS_P (value, size)))             \
    {                                                         \
    mf_state resume_state = old_state;                        \
    __mf_state = old_state;                                   \
    __mf_check ((unsigned)value, size, __FILE__ ":" STRINGIFY(__LINE__) " (" __PRETTY_FUNCTION__ ")" );                       \
    old_state = resume_state;                                 \
    }                                                         \
 }


#define BEGIN_PROTECT(ty, fname, ...)       \
  ty result;                                \
  mf_state old_state;                       \
  if (UNLIKELY (__mf_state != active))      \
    return __real_ ## fname (__VA_ARGS__);  \
  else                                      \
  {                                         \
     TRACE_IN;                              \
     old_state = __mf_state;                \
     __mf_state = reentrant;                \
  }

#define END_PROTECT(ty, fname, ...)              \
  result = (ty) __real_ ## fname (__VA_ARGS__);  \
  __mf_state = old_state;                        \
  TRACE_OUT;                                     \
  return result;



/* {{{ malloc/free etc. */

void *
__wrap_malloc (size_t c)
{
  extern void * __real_malloc (size_t);
  size_t size_with_crumple_zomes;

  BEGIN_PROTECT (void *, malloc, c);
  
  size_with_crumple_zomes = c + (2 * __mf_opts.crumple_zone);
  result = (char *) __real_malloc (size_with_crumple_zomes);
  
  __mf_state = old_state;
  
  if (LIKELY(result))
    {
      result += __mf_opts.crumple_zone;
      __mf_register ((uintptr_t) result, c,
		     __MF_TYPE_HEAP, "malloc region");
    }

  TRACE_OUT;
  return result;
}


void *
__wrap_calloc (size_t c, size_t n)
{
  extern void * __real_calloc (size_t c, size_t);
  extern void * __real_malloc (size_t c);
  extern void *__real_memset (void *s, int c, size_t n);
  size_t size_with_crumple_zones;

  BEGIN_PROTECT (char *, calloc, c, n);
  
  size_with_crumple_zones = (c * n) + (2 * __mf_opts.crumple_zone);  
  result = (char *) __real_malloc (size_with_crumple_zones);
  
  if (LIKELY(result))
    __real_memset (result, 0, size_with_crumple_zones);
  
  __mf_state = old_state;
  
  if (LIKELY(result))
    {
      result += __mf_opts.crumple_zone;
      __mf_register ((uintptr_t) result, c*n, 
		     __MF_TYPE_HEAP, "calloc region");
    }
  
  TRACE_OUT;
  return result;
}

void *
__wrap_realloc (void *buf, size_t c)
{
  extern void * __real_realloc (void *, size_t);
  size_t size_with_crumple_zones;
  char *base = buf;

  BEGIN_PROTECT (char *, realloc, buf, c);

  if (LIKELY(buf))
    base -= __mf_opts.crumple_zone;

  size_with_crumple_zones = c + (2 * __mf_opts.crumple_zone);
  result = (char *) __real_realloc (base, size_with_crumple_zones);

  __mf_state = old_state;      

  if (LIKELY(buf))
    __mf_unregister ((uintptr_t) buf, 0);
  
  if (LIKELY(result))
    {
      result += __mf_opts.crumple_zone;
      __mf_register ((uintptr_t) result, c, 
		     __MF_TYPE_HEAP, "realloc region");
    }

  TRACE_OUT;
  return result;
}


void
__wrap_free (void *buf)
{
  /* Use a circular queue to delay some number (__mf_opts.free_queue_length) of free()s.  */
  static void *free_queue [__MF_FREEQ_MAX];
  static unsigned free_ptr = 0;
  static int freeq_initialized = 0;
  mf_state old_state;
  extern void * __real_free (void *);  
  extern void *__real_memset (void *s, int c, size_t n);

  if (UNLIKELY (__mf_state != active))
    {
      __real_free (buf);
      return;
    }

  if (UNLIKELY(!freeq_initialized))
    {
      __real_memset (free_queue, 0, 
		     __MF_FREEQ_MAX * sizeof (void *));
      freeq_initialized = 1;
    }

  if (UNLIKELY(buf == NULL))
    return;

  TRACE_IN;

  __mf_unregister ((uintptr_t) buf, 0);

  old_state = __mf_state;
  __mf_state = reentrant;
      
  if (UNLIKELY(__mf_opts.free_queue_length > 0))
    {
      
      if (free_queue [free_ptr] != NULL)
	{
	  char *base = free_queue [free_ptr];
	  base -= __mf_opts.crumple_zone;
	  if (__mf_opts.trace_mf_calls)
	    {
	      fprintf (stderr, "mf: freeing deferred pointer #%d %p = %p - %d\n", 
		       __mf_opts.free_queue_length, base, 
		       free_queue [free_ptr], __mf_opts.crumple_zone);
	    }
	  __real_free (base);
	}
      free_queue [free_ptr] = buf;
      free_ptr = (free_ptr == (__mf_opts.free_queue_length-1) ? 0 : free_ptr + 1);
    } 
  else 
    {
      /* back pointer up a bit to the beginning of crumple zone */
      char *base = (char *)buf;
      base -= __mf_opts.crumple_zone;
      if (__mf_opts.trace_mf_calls)
	{
	  fprintf (stderr, "mf: freeing pointer %p = %p - %d\n", base, 
		   buf, __mf_opts.crumple_zone);
	}
      __real_free (base);
    }
  
  __mf_state = old_state;
  TRACE_OUT;
}

void *
__wrap_dlopen (const char *filename, int flag)
{
  extern void * __real_dlopen(const char *filename, int flag);
  BEGIN_PROTECT(void *, dlopen, filename, flag);
  result = __real_dlopen (filename, flag);
  __mf_state = old_state;
  __mf_init_heuristics ();
  TRACE_OUT;
  return result;
}


void  *  
__wrap_mmap(void  *start,  size_t length, int prot , 
	    int flags, int fd, off_t offset)
{

  extern void *__real_mmap (void *, size_t, int, 
			    int, int, off_t);
  BEGIN_PROTECT(void *, mmap, start, length, 
		prot, flags, fd, offset);

  result = __real_mmap (start, length, prot, 
			flags, fd, offset);

  __mf_state = old_state;

  if ((uintptr_t)result != -1)
    {
      __mf_register ((uintptr_t) result, 
		     (uintptr_t) result + length - 1,
		     __MF_TYPE_GUESS, 
		     "(heuristic) mmap region");
    }

  TRACE_OUT;
  return result;
}

int 
__wrap_munmap(void *start, size_t length)
{
  extern void *__real_munmmap (void *, size_t);
  BEGIN_PROTECT(int, munmap, start, length);
  
  result = __real_munmap (start, length);

  __mf_state = old_state;
  __mf_unregister ((uintptr_t)start, length);
}

/* }}} */
/* {{{ str*,mem*,b* */

void *
__wrap_memcpy (void *dest, const void *src, size_t n)
{
  extern void *__real_memcpy (void *dest, const void *src, size_t n);
  BEGIN_PROTECT(void *, memcpy, dest, src, n);
  MF_VALIDATE_EXTENT(src, n);
  MF_VALIDATE_EXTENT(dest, n);
  END_PROTECT (void *, memcpy, dest, src, n);
}



void *
__wrap_memmove (void *dest, const void *src, size_t n)
{
  extern void *__real_memmove (void *dest, const void *src, size_t n);
  BEGIN_PROTECT(void *, memmove, dest, src, n);
  MF_VALIDATE_EXTENT(src, n);
  MF_VALIDATE_EXTENT(dest, n);
  END_PROTECT(void *, memmove, dest, src, n);
}


void *
__wrap_memset (void *s, int c, size_t n) 
{
  extern void *__real_memset (void *s, int c, size_t n);
  BEGIN_PROTECT(void *, memset, s, c, n);
  MF_VALIDATE_EXTENT(s, n);
  END_PROTECT(void *, memset, s, c, n);
}

int
__wrap_memcmp (const void *s1, const void *s2, size_t n)
{
  extern int __real_memcmp (const void *s1, const void *s2, size_t n);
  BEGIN_PROTECT(int, memcmp, s1, s2, n);
  MF_VALIDATE_EXTENT(s1, n);
  MF_VALIDATE_EXTENT(s2, n);
  END_PROTECT(int, memcmp, s1, s2, n);
}

void *
__wrap_memchr (const void *s, int c, size_t n)
{
  extern void *__real_memchr (const void *s, int c, size_t n);
  BEGIN_PROTECT(void *, memchr, s, c, n);
  MF_VALIDATE_EXTENT(s, n);
  END_PROTECT(void *, memchr, s, c, n);
}

void *
__wrap_memrchr (const void *s, int c, size_t n)
{
  extern void *__real_memrchr (const void *s, int c, size_t n);
  BEGIN_PROTECT(void *, memrchr, s, c, n);
  MF_VALIDATE_EXTENT(s, n);
  END_PROTECT(void *, memrchr, s, c, n);
}


char *
__wrap_strcpy (char *dest, const char *src)
{
  extern char *__real_strcpy (char *dest, const char *src);
  int n;

  /* nb: just because strlen(src) == n doesn't mean (src + n) or (src + n +
     1) are valid pointers. the allocated object might have size < n.
     check anyways. */

  BEGIN_PROTECT(char *, strcpy, dest, src); 
  TRACE("mf: strcpy %p -> %p\n", src, dest);
  n = __real_strlen (src);
  MF_VALIDATE_EXTENT(src, (n + 1)); 
  MF_VALIDATE_EXTENT(dest, (n + 1));
  END_PROTECT(char *, strcpy, dest, src);
}

char *
__wrap_strncpy (char *dest, const char *src, size_t n)
{
  extern char *__real_strncpy (char *dest, const char *src, size_t n);
  int len;

  BEGIN_PROTECT(char *, strncpy, dest, src, n);
  TRACE("mf: strncpy %d chars %p -> %p\n", n, src, dest);
  len = __real_strnlen (src, n);
  MF_VALIDATE_EXTENT(src, len);
  MF_VALIDATE_EXTENT(dest, len); /* nb: strNcpy */
  END_PROTECT(char *, strncpy, dest, src, n);
}

char *
__wrap_strcat (char *dest, const char *src)
{
  extern char *__real_strcat (char *dest, const char *src);
  int dest_sz;
  int src_sz;

  BEGIN_PROTECT(char *, strcat, dest, src);
  dest_sz = __real_strlen (dest);
  src_sz = __real_strlen (src);  
  MF_VALIDATE_EXTENT(src, (src_sz + 1));
  MF_VALIDATE_EXTENT(dest, (dest_sz + src_sz + 1));
  END_PROTECT(char *, strcat, dest, src);
}

char *
__wrap_strncat (char *dest, const char *src, size_t n)
{

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

  extern char *__real_strncat (char *dest, const char *src, size_t n);
  int src_sz;
  int dest_sz;

  BEGIN_PROTECT(char *, strncat, dest, src, n);
  src_sz = __real_strnlen (src, n);
  dest_sz = __real_strnlen (dest, n);
  MF_VALIDATE_EXTENT(src, src_sz);
  MF_VALIDATE_EXTENT(dest, (dest_sz + src_sz + 1));
  END_PROTECT(char *, strncat, dest, src, n);
}

int
__wrap_strcmp (const char *s1, const char *s2)
{
  extern int __real_strcmp (const char *s1, const char *s2);
  int s1_sz;
  int s2_sz;

  BEGIN_PROTECT(int, strcmp, s1, s2);
  s1_sz = __real_strlen (s1);
  s2_sz = __real_strlen (s2);  
  MF_VALIDATE_EXTENT(s1, s1_sz+1);
  MF_VALIDATE_EXTENT(s2, s2_sz+1);
  END_PROTECT(int, strcmp, s1, s2);
}

int
__wrap_strcasecmp (const char *s1, const char *s2)
{
  extern int __real_strcasecmp (const char *s1, const char *s2);
  int s1_sz;
  int s2_sz;

  BEGIN_PROTECT(int, strcasecmp, s1, s2);
  s1_sz = __real_strlen (s1);
  s2_sz = __real_strlen (s2);  
  MF_VALIDATE_EXTENT(s1, s1_sz+1);
  MF_VALIDATE_EXTENT(s2, s2_sz+1);
  END_PROTECT(int, strcasecmp, s1, s2);
}

int
__wrap_strncmp (const char *s1, const char *s2, size_t n)
{
  extern int __real_strncmp (const char *s1, const char *s2, size_t n);
  int s1_sz;
  int s2_sz;

  BEGIN_PROTECT(int, strncmp, s1, s2, n);
  s1_sz = __real_strnlen (s1, n);
  s2_sz = __real_strnlen (s2, n);
  MF_VALIDATE_EXTENT(s1, s1_sz);
  MF_VALIDATE_EXTENT(s2, s2_sz);
  END_PROTECT(int, strncmp, s1, s2, n);
}

int
__wrap_strncasecmp (const char *s1, const char *s2, size_t n)
{
  extern int __real_strncasecmp (const char *s1, const char *s2, size_t n);
  int s1_sz;
  int s2_sz;

  BEGIN_PROTECT(int, strncasecmp, s1, s2, n);
  s1_sz = __real_strnlen (s1, n);
  s2_sz = __real_strnlen (s2, n);
  MF_VALIDATE_EXTENT(s1, s1_sz);
  MF_VALIDATE_EXTENT(s2, s2_sz);
  END_PROTECT(int, strncasecmp, s1, s2, n);
}

char *
__wrap_strdup (const char *s)
{
  extern char *__real_strdup (const char *s);
  extern void *__real_malloc (size_t sz);
  int n;

  BEGIN_PROTECT(char *, strdup, s);
  n = __real_strlen (s);
  MF_VALIDATE_EXTENT(s, n+1);

  result = (char *)__real_malloc (n+1 + 2*(__mf_opts.crumple_zone));

  if (UNLIKELY(!result))
    {
      __mf_state = old_state;
      TRACE_OUT;
      return result;
    }

  result += __mf_opts.crumple_zone;
  __real_memcpy (result, s, n);
  result[n] = '\0';

  __mf_register ((uintptr_t) result, n+1, __MF_TYPE_HEAP, "strdup region");
  __mf_state = old_state;
  TRACE_OUT;
  return result;
}

char *
__wrap_strndup (const char *s, size_t n)
{
  extern char *__real_strndup (const char *s, size_t n);
  extern void *__real_malloc (size_t sz);
  int sz;

  BEGIN_PROTECT(char *, strndup, s, n);
  sz = __real_strnlen (s, n);
  MF_VALIDATE_EXTENT(s, sz); /* nb: strNdup */

  /* note: strndup still adds a \0, even with the N limit! */
  result = (char *)__real_malloc (n+1 + 2*(__mf_opts.crumple_zone));

  if (UNLIKELY(!result))
    {
      __mf_state = old_state;
      TRACE_OUT;
      return result;
    }

  result += __mf_opts.crumple_zone;
  __real_memcpy (result, s, n);
  result[n] = '\0';

  __mf_register ((uintptr_t) result, n+1, __MF_TYPE_HEAP, "strndup region");
  __mf_state = old_state;
  TRACE_OUT;
  return result;
}


char *
__wrap_strchr (const char *s, int c)
{
  extern char *__real_strchr (const char *s, int c);
  int n;

  BEGIN_PROTECT(char *, strchr, s, c);
  n = __real_strlen (s);
  MF_VALIDATE_EXTENT(s, n+1);
  END_PROTECT(char *, strchr, s, c);
}

char *
__wrap_strrchr (const char *s, int c)
{
  extern char *__real_strrchr (const char *s, int c);
  int n;

  BEGIN_PROTECT(char *, strrchr, s, c);
  n = __real_strlen (s);
  MF_VALIDATE_EXTENT(s, n+1);
  END_PROTECT(char *, strrchr, s, c);
}


char *
__wrap_strstr (const char *haystack, const char *needle)
{
  extern char *__real_strstr (const char *haystack, const char *needle);
  int haystack_sz;
  int needle_sz;

  BEGIN_PROTECT(char *, strstr, haystack, needle);
  haystack_sz = __real_strlen (haystack);
  needle_sz = __real_strlen (needle);
  MF_VALIDATE_EXTENT(haystack, haystack_sz+1);
  MF_VALIDATE_EXTENT(needle, needle_sz+1);
  END_PROTECT(char *, strstr, haystack, needle);
}


void *
__wrap_memmem (const void *haystack, size_t haystacklen,
	       const void *needle, size_t needlelen)
{
  extern void *__real_memmem (const void *haystack, size_t haystacklen,
			      const void *needle, size_t needlelen);

  BEGIN_PROTECT(void *, memmem, haystack, haystacklen, needle, needlelen);
  MF_VALIDATE_EXTENT(haystack, haystacklen);
  MF_VALIDATE_EXTENT(needle, needlelen);
  END_PROTECT(char *, memmem, haystack, haystacklen, needle, needlelen);
}

int
__wrap_strlen (const char *s)
{
  extern int __real_strlen (const char *s);

  BEGIN_PROTECT(int, strlen, s);
  result = __real_strlen (s);
  MF_VALIDATE_EXTENT(s, result+1);
  __mf_state = old_state;
  TRACE_OUT;
  return result;
}

int
__wrap_strnlen (const char *s, size_t n)
{
  extern int __real_strnlen (const char *s, size_t n);

  BEGIN_PROTECT(int, strnlen, s, n);
  result = __real_strnlen (s, n);
  MF_VALIDATE_EXTENT(s, result);
  __mf_state = old_state;
  TRACE_OUT;
  return result;
}

void
__wrap_bzero (void *s, size_t n)
{
  extern void __real_bzero (void *s, size_t n);
  mf_state old_state;

  if (UNLIKELY (__mf_state != active))
    {
      __real_bzero (s, n);
      return;
    }
  TRACE_IN;
  old_state = __mf_state;
  __mf_state = reentrant;
  MF_VALIDATE_EXTENT(s, n);
  __real_bzero (s, n);
  __mf_state = old_state;
  TRACE_OUT;
}

void
__wrap_bcopy (const void *src, void *dest, size_t n)
{
  extern void __real_bcopy (const void *src, void *dest, size_t n);
  mf_state old_state;

  if (UNLIKELY (__mf_state != active))
    {
      __real_bcopy (src, dest, n);
      return;
    }
  TRACE_IN;
  old_state = __mf_state;
  __mf_state = reentrant;
  MF_VALIDATE_EXTENT(src, n);
  MF_VALIDATE_EXTENT(dest, n);
  __real_bcopy (src, dest, n);
  __mf_state = old_state;
  TRACE_OUT;
}

int
__wrap_bcmp (const void *s1, const void *s2, size_t n)
{
  extern int __real_bcmp (const void *s1, const void *s2, size_t n);

  BEGIN_PROTECT(int, bcmp, s1, s2, n);
  MF_VALIDATE_EXTENT(s1, n);
  MF_VALIDATE_EXTENT(s2, n);
  END_PROTECT(int, bcmp, s1, s2, n);
}

char *
__wrap_index (const char *s, int c)
{
  extern char *__real_index (const char *s, int c);
  int n;

  BEGIN_PROTECT(char *, index, s, c);
  n = __real_strlen (s);
  MF_VALIDATE_EXTENT(s, n+1);
  END_PROTECT(char *, index, s, c);
}



char *
__wrap_rindex (const char *s, int c)
{
  extern char *__real_rindex (const char *s, int c);
  int n;

  BEGIN_PROTECT(char *, rindex, s, c);
  n = __real_strlen (s);
  MF_VALIDATE_EXTENT(s, n+1);
  END_PROTECT(char *, rindex, s, c);
}

/* 

general form: 

void
__wrap_ ()
{
  extern void __real_ ();
  MF_VALIDATE_EXTENT();
  result = __real_ ();
  __mf_state = old_state;
  return result;
}

*/


/* }}} */
/* {{{ *printf,*scanf */



/* }}} */

/* ------------------------------------------------------------------------ */
