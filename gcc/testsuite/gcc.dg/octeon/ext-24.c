/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\texts\t"  } } */
/* { dg-final { scan-assembler-not "sll\t" } } */

struct s
{
  unsigned int a:10;
  unsigned long long d:32;
  unsigned int e:3;
};
                                                                                  
unsigned int f (struct s s, unsigned a)
{
  return s.d + a;
}
