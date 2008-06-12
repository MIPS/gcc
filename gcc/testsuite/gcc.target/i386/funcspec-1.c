/* Test whether using target specific options, we can generate SSE2 code on
   32-bit, which does not generate SSE2 by default.  */
/* { dg-do compile } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-O3 -ftree-vectorize -march=i386" } */
/* { dg-final { scan-assembler "addps\[ \t\]" } } */

#ifndef SIZE
#define SIZE 1024
#endif

static float a[SIZE] __attribute__((__aligned__(16)));
static float b[SIZE] __attribute__((__aligned__(16)));
static float c[SIZE] __attribute__((__aligned__(16)));

void addnums (void) __attribute__ ((__option__ ("sse2")));

void
addnums (void)
{
  int i = 0;
  for (; i < SIZE; ++i)
    a[i] = b[i] + c[i];
}
