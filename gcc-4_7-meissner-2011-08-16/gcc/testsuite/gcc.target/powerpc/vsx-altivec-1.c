/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_vsx_ok } */
/* { dg-options "-mvsx -Ofast" } */

#include <stddef.h>
#include <stdlib.h>

#ifndef TYPE
#define TYPE vector int
#endif

/* Test whether Altivec lvx, stvx can be generated under VSX.  There should be
   2 Altivec loads, 2 VSX loads, 2 Altivec stores, and 2 VSX stores
   generated.  */

void
load_with_and (TYPE *a, TYPE *b)
{
  *a = *((TYPE *)(((ptrdiff_t) b) & (ptrdiff_t)-16));
}

void
load2_with_and (TYPE *a, TYPE *b, ptrdiff_t c)
{
  *a = *((TYPE *)((((ptrdiff_t) b) + c) & (ptrdiff_t)-16));
}

void
store_with_and (TYPE *a, TYPE *b)
{
  *((TYPE *)(((ptrdiff_t) a) & (ptrdiff_t)-16)) = *b;
}

void
store2_with_and (TYPE *a, TYPE *b, ptrdiff_t c)
{
  *((TYPE *)((((ptrdiff_t) a) + c) & (ptrdiff_t)-16)) = *b;
}

/* { dg-final { scan-assembler-times "lvx" 2 } } */
/* { dg-final { scan-assembler-times "stvx" 2 } } */
/* { dg-final { scan-assembler-times "lxvw4x\|lxvd2x" 2 } } */
/* { dg-final { scan-assembler-times "stxvw4x\|stxvd2x" 2 } } */

