#ifndef _IMMINTRIN_H_INCLUDED
# error "Never use <plintrin.h> directly; include <immintrin.h> instead."
#endif

//typedef __int128 __bnd;
/*
extern __inline __bnd __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__pl_bndmk (void* p, int b)
{
#ifdef  __x86_64__
  return __builtin_ia32_bndmk64 (p, b);
#else
  return __builtin_ia32_bndmk32 (p, b);
#endif
}

extern __inline void  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__pl_bndstx (void* p1, void* p2, __bnd b)
{
#ifdef  __x86_64__
  return __builtin_ia32_bndstx64 (p1, p2, b);
#else
  return __builtin_ia32_bndstx32 (p1, p2, b);
#endif
}

extern __inline void  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__pl_bndldx (__bnd b, void* p1, void* p2)
{
#ifdef  __x86_64__
  return __builtin_ia32_bndstx64 (b, p1, p2);
#else
  return __builtin_ia32_bndstx32 (b, p1, p2);
#endif
}

extern __inline void  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__pl_bndcl (__bnd b, void* p1) 
{ 
#ifdef  __x86_64__ 
  return __builtin_ia32_bndcl64 (b, p1); 
#else 
  return __builtin_ia32_bndcl32 (b, p1); 
#endif 
}

extern __inline void  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__pl_bndcu (__bnd b, void* p1)           
{
#ifdef  __x86_64__ 
  return __builtin_ia32_bndcu64 (b, p1);     
#else
  return __builtin_ia32_bndcu32 (b, p1);     
#endif
}
*/
extern __inline void *  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__pl_bind_bounds (void *p, void* lb, size_t size)
{
#ifdef  __x86_64__
  return __builtin_ia32_bind_bounds64 (p, lb, size);
#else
  return __builtin_ia32_bind_bounds32 (p, lb, size);
#endif
}

extern __inline void *  __attribute__((__gnu_inline__, __always_inline__, __artificial__))
__pl_intersect_bounds (void *p, void* lb, size_t size)
{
#ifdef  __x86_64__
  return __builtin_ia32_intersect_bounds64 (p, lb, size);
#else
  return __builtin_ia32_intersect_bounds32 (p, lb, size);
#endif
}
