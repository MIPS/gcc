#ifndef __MF_RUNTIME_H
#define __MF_RUNTIME_H

/* ------------------------------------------------------------------------ */
/* Public libmudflap declarations */

#ifndef HAVE_UINTPTR_T
#define HAVE_UINTPTR_T
typedef unsigned long uintptr_t;
#endif

#define MINPTR ((uintptr_t) 0)
#define MAXPTR (~ (uintptr_t) 0)

/* Clamp the addition/subtraction of uintptr_t's to [MINPTR,MAXPTR] */
#define CLAMPSUB(ptr,offset) ((ptr) >= (offset) ? (ptr)-(offset) : MINPTR)
#define CLAMPADD(ptr,offset) ((ptr) <= MAXPTR-(offset) ? (ptr)+(offset) : MAXPTR)
#define CLAMPSZ(ptr,size) ((size) ? ((ptr) <= MAXPTR-(size)+1 ? (ptr)+(size)-1 : MAXPTR) : (ptr))

struct __mf_cache { uintptr_t low; uintptr_t high; };
extern struct __mf_cache __mf_lookup_cache [];
extern uintptr_t __mf_lc_mask;
extern unsigned char __mf_lc_shift;
#define __MF_CACHE_INDEX(ptr) ((((uintptr_t) (ptr)) >> __mf_lc_shift) & __mf_lc_mask)
#define __MF_CACHE_MISS_P(ptr,sz) ({ \
             struct __mf_cache *elem = & __mf_lookup_cache[__MF_CACHE_INDEX((ptr))]; \
             ((elem->low > (uintptr_t) (ptr)) ||                  \
	      (elem->high < (CLAMPADD((uintptr_t) (ptr), (uintptr_t) CLAMPSUB(sz,1) )))); })
/* XXX: the above should use CLAMPSZ () */
extern void __mf_check (uintptr_t ptr, uintptr_t sz, const char *location);


/* Codes to describe the context in which a violation occurs. */

#define __MF_VIOL_UNKNOWN 0
#define __MF_VIOL_CHECK 1
#define __MF_VIOL_REGISTER 2
#define __MF_VIOL_UNREGISTER 3

/* Codes to describe a region of memory being registered. */
  
#define __MF_TYPE_UNKNOWN 0
#define __MF_TYPE_HEAP 1
#define __MF_TYPE_STACK 2
#define __MF_TYPE_STATIC 3
#define __MF_TYPE_GUESS 4
#define __MF_TYPE_MAX __MF_TYPE_GUESS

/* The "public" mudflap API */
extern void __mf_register (uintptr_t ptr, uintptr_t sz, int type, const char *name);
extern void __mf_unregister (uintptr_t ptr, uintptr_t sz);
extern void __mf_report ();

#endif
