/* { dg-options "-m32 -march=32r6" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

struct S
{
  char a;
  int b;
  long c;
} __attribute__ ((packed)) S;

long
foo (struct S *s)
{
  long a = s->c;
  return a;
}

/* { dg-final { scan-assembler "\tualw\\s\\\$a0,5\\\(\\\$a0\\\)\n" } } */
