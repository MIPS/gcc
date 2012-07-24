/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "ins\t" } } */
/* Not need to sign-extend.  Chars are passed as int.  */
/* { dg-final { scan-assembler-not "\tseb\t"  } } */
/* There should be no other sign/zero extends in the output. */
/* { dg-final { scan-assembler-not "\texts\t" } } */
/* { dg-final { scan-assembler-not "\tsll\t" } } */

struct s
{
  int a:2;
  int b:21;
};

struct s
f (struct s s, char a)
{
  s.b = a;
  return s;
}
