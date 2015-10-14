/* { dg-do compile { target fpic } } */
/* { dg-options "-O2 -fPIC" } */

typedef __SIZE_TYPE__ size_t;
extern void *memset (void *, int, size_t)
  __attribute__ ((visibility ("hidden")));
extern void *memset (void *, int, size_t);

void
bar (void *s, size_t n)
{
  __builtin_memset (s, '\0', n);
}

/* { dg-final { scan-assembler-not "memset@PLT" { target i?86-*-* x86_64-*-* } } } */
