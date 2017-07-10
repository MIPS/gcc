/* { dg-options "-mips16 -mabi=32" } */

__attribute__((nomips16, always_inline, inline_intermix))
inline int foo()
{
  return 1;
}

int bar()
{
  return foo();
}

