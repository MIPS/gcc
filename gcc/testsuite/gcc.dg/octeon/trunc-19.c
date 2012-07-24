/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "sll\t" } } */

struct c
{
  int *j;
  int a:8;
};

void
f (struct c *c, struct c *d)
{
  d->a = c->a;
}
