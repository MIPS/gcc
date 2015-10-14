/* { dg-do compile { target fpic } } */
/* { dg-options "-O2 -fPIC" } */

typedef __SIZE_TYPE__ size_t;
extern void *memcpy (void *, const void *, size_t);
extern void *memcpy (void *, const void *, size_t)
  __attribute__ ((visibility ("hidden")));

void
bar (void *d, void *s, size_t n)
{
  memcpy (d, s, n);
}

/* { dg-final { scan-assembler-not "memcpy@PLT" { target i?86-*-* x86_64-*-* } } } */
