/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-march=32r6" } */
/* { dg-final { scan-assembler-times "\tuasw " 1 } } */
/* { dg-final { scan-assembler-times "\tualw " 1 } } */
/* { dg-final { scan-assembler-not "\tnop" } } */

/* Test to make sure we produce the unaligned load/store for
   both 64bit and 32bits sized accesses.  */

#include <stdint.h>

struct S
{
  char c;
  int i;
    uint16_t s;

  long long l;

} __attribute__ ((packed)) S __attribute__((aligned(1) ));


void
uasw_insn (int i)
{
  S.i = i;
}

int
ualw_insn ()
{
  return S.i;
}
