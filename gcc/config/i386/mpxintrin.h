#ifndef _IMMINTRIN_H_INCLUDED
# error "Never use <plintrin.h> directly; include <immintrin.h> instead."
#endif

#include <stddef.h>

extern __inline void *  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__mpx_bind_bounds (void *p, void* lb, size_t size)
{
  return __builtin_ia32_bind_bounds (p, lb, size);
}

extern __inline void *  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__mpx_intersect_bounds (void *p, void* lb, size_t size)
{
  return __builtin_ia32_intersect_bounds (p, lb, size);
}

extern void __mpx_check_address_read (const void *);
extern void __mpx_check_address_write (const void *);
