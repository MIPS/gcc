/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ins\t" } } */
/* No need to sign-extend.  Shorts are passed as int.  */
/* { dg-final { scan-assembler-not "\tseh\t"  } } */

struct s
{
  int a:2;
  int b:21;
};

struct s
f (struct s s, short a)
{
  s.b = a;
  return s;
}
