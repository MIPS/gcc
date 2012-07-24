/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts\t" } } */

struct s
{
  int a:1;
  int b:10;
};

int f ()
{
  struct s s1;
  asm ("" : "=r"(s1));
  return (int) s1.b + 1;
}
