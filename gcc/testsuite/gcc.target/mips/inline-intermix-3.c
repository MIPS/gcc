/* { dg-options "-mips16 -mabi=32 -minline-intermix" } */

__attribute__((nomips16, always_inline))
inline int foo()
{
  return 1;
}

int bar()
{
  return foo();
}

