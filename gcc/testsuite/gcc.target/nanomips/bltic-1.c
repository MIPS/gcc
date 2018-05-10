/* { dg-options "-m32 isa_rev>=6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-skip-if "code quality test" { *-*-* } { "-Os" } { "" } } */

short __attribute__((noinline))
fn1(int p1, int p2) { 
  return p2 >= 2 ? p1 : p1 > p2; 
}

/* { dg-final { scan-assembler "\tbltic\t\\\$" } } */

