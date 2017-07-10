/* { dg-options "-mips16 -mabi=32" } */

__attribute__((nomips16, always_inline))
inline int foo() /* { dg-error "target specific option mismatch" } */
{
  return 1;
}

int bar()
{
  return foo();
}

