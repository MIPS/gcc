/* { dg-options "-m32 -mdspr3 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0"} { "" } } */

struct foo { int a; };

void bar(struct foo *p)
{
  if (__builtin_expect(p->a >= 52, 0))
    __builtin_trap();
}

/* { dg-final { scan-assembler "\tteq\t" } } */

