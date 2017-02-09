/* { dg-options "-mabi=p32 -mxlp isa_rev>=7" } */
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

/* { dg-final { scan-assembler "\tualw\\s\\\$4,5\\\(\\\$4\\\)\n" } } */
