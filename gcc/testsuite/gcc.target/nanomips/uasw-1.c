/* { dg-options "-m32 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

struct S
{
  char a;
  int b;
  long c;
} __attribute__ ((packed)) S;

void
foo (struct S *s)
{
  s->c = 0;
}

/* { dg-final { scan-assembler "\tuasw\\s\\\$zero,5\\\(\\\$a0\\\)\n" } } */
