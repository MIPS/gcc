#ifndef _IMMINTRIN_H_INCLUDED
# error "Never use <plintrin.h> directly; include <immintrin.h> instead."
#endif

//typedef __int128 __bnd;

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
