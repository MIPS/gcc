/* { dg-options "-O2 -mgp64" } */
/* { dg-final { scan-assembler-not "andi\t" } } */
/* { dg-final { scan-assembler-times "or\t" 1 } } */
/* { dg-final { scan-assembler-times "ins\t" 1 } } */

struct B {
  unsigned :31;
  unsigned bit1 : 1;
};

void
foo (struct B *b, struct B *c)
{
  b->bit1 = b->bit1 | c->bit1;
}

