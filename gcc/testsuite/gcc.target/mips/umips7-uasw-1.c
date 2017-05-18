/* { dg-options "-mabi=p32 -mxlp isa_rev>=7" } */
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

/* { dg-final { scan-assembler "\tuasw\\s\\\$0,5\\\(\\\$4\\\)\n" } } */
