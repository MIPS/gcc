/* { dg-do compile } */
/* { dg-options "-Wno-psabi -msse4 -m3dnow -m3dnowa -mno-mmx" } */

typedef int v2si __attribute__ ((__vector_size__ (8)));
typedef short v4hi __attribute__ ((__vector_size__ (8)));
typedef char v8qi __attribute__ ((__vector_size__ (8)));
typedef float v2sf __attribute__ ((__vector_size__ (8)));
typedef double v2df __attribute__ ((__vector_size__ (16)));
typedef float v4sf __attribute__ ((__vector_size__ (16)));

v4hi
foo1 (v4hi x, v4hi y)
{
  return  __builtin_ia32_pmulhuw (x, y); /* { dg-error "needs isa option\[^\n\r]*-msse -m3dnowa -mmmx" } */
}

v2sf
foo2 (v2sf x, v2sf y)
{
  return __builtin_ia32_pfpnacc (x, y); /* { dg-error "needs isa option\[^\n\r]*-m3dnowa -mmmx" } */
}

v2sf
foo3 (v2sf x, v2sf y)
{
   return __builtin_ia32_pfmul (x, y); /* { dg-error "needs isa option\[^\n\r]*-m3dnow -mmmx" } */
}

v4hi
foo4 (v4hi x, v4hi y)
{
  return __builtin_ia32_psubusw (x, y); /* { dg-error "needs isa option\[^\n\r]*-mmmx" } */
}

v2si
foo5 (v2df x)
{
  return __builtin_ia32_cvtpd2pi (x); /* { dg-error "needs isa option\[^\n\r]*-msse2 -mmmx" } */
}

v4sf
foo6 (v4sf x, v2si y)
{
  return __builtin_ia32_cvtpi2ps (x, y); /* { dg-error "needs isa option\[^\n\r]*-msse -mmmx" } */
}

v4hi
foo7 (v4hi x, v4hi y)
{
  return __builtin_ia32_phaddw (x, y); /* { dg-error "needs isa option\[^\n\r]*-mssse3 -mmmx" } */
}
