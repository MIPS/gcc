/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -fno-merge-const-bfstores" } */
/* { dg-final { scan-assembler {0x123} } } */

/* Even though merge const bfstores is off, this should
   pass as RTL CSE should be able to merge the constant
   stores still. */

struct s
{
  long long a:4;
  long long b:4;
  long long c:4;
  long long d:52;
};

f ()
{
  struct s s;
  s.a = 1;
  s.b = 2;
  s.c = 3;
  g (s);
}
