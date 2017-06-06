/* { dg-options "-mxlp -mhard-float" }
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-times "\tuasw " 3 } } */
/* { dg-final { scan-assembler-times "\tualw " 3 } } */
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
sd (long long l)
{
  S.l = l;
}

long long
ld ()
{
  return S.l;
}

void
sw (int i)
{
  S.i = i;
}

int
lw ()
{
  return S.i;
}
