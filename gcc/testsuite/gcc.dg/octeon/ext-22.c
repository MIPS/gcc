/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ext\t" } } */

struct s
{
  unsigned int a:10;
  unsigned int d:17;
  unsigned int e:3;
};
                                                                                  
unsigned int f ()
{
  struct s s;
  asm ("" : "=r"(s));
  return s.d;
}
