#ifndef __MF_RUNTIME_H
#define __MF_RUNTIME_H


#ifndef HAVE_UINTPTR_T
#define HAVE_UINTPTR_T
typedef unsigned long uintptr_t;
#endif

struct __mf_cache { uintptr_t low; uintptr_t high; };

extern struct __mf_cache __mf_lookup_cache [];
extern uintptr_t __mf_lc_mask;
extern unsigned char __mf_lc_shift;

#define __MF_CACHE_INDEX(ptr) ((((uintptr_t) (ptr)) >> __mf_lc_shift) & __mf_lc_mask)

extern void __mf_check (uintptr_t ptr, uintptr_t sz);
#define __MF_VIOL_UNKNOWN 0
#define __MF_VIOL_CHECK 1
#define __MF_VIOL_REGISTER 2
#define __MF_VIOL_UNREGISTER 3
extern void __mf_violation (uintptr_t ptr, uintptr_t sz, uintptr_t pc, int type);
#define __MF_TYPE_UNKNOWN 0
#define __MF_TYPE_HEAP 1
#define __MF_TYPE_STACK 2
#define __MF_TYPE_STATIC 3
extern void __mf_register (uintptr_t ptr, uintptr_t sz, int type, const char *name);
extern void __mf_unregister (uintptr_t ptr, uintptr_t sz);
extern void __mf_report ();


#endif
