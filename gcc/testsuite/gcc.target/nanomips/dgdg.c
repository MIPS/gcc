/* { dg-do compile } */
/* { dg-options "-mgp32 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\tins\t" } } */

struct
{
  unsigned int i : 2;
  unsigned int j : 3;
  unsigned int k : 4;
} s;

void
foo (void)
{
  s.j = 1;
}
