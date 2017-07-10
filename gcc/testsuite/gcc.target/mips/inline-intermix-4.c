/* { dg-options "-mips16 -mabi=32 -minline-intermix" } */

__attribute__((nomips16, always_inline, inline_intermix("no")))
inline int foo() /* { dg-error "target specific option mismatch" } */
{
  return 1;
}

int bar()
{
  return foo();
}

