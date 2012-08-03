/* { dg-do compile } */
/* { dg-options " isa_rev>=2" } */
/* { dg-final { scan-assembler-times "\tins\t" 2 } } */
/* { dg-final { scan-assembler-not "\tand\t" } } */

struct a
{
  unsigned a:3;
  unsigned b:2;
};


void f(struct a *b, int c)
{
  b->a = c;
  b->b = c;
}

