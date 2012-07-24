/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tins\t" } } */

union u
{
  unsigned char a;
  int b;
};
int a;
union u
foo (int c)
{
  union u b;
  b.b = c;
  if (a)
    b.a = 1;
  else
    b.a = 0;
  return b;
}
