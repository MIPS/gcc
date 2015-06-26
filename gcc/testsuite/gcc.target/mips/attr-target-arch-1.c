/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32 -mabi=32" } */

/* { dg-final { scan-assembler "test_arch1.*andi.*test_arch1" } } */
/* { dg-final { scan-assembler "test_arch2.*ext.*test_arch2" } } */

struct
{
  unsigned long a:10;
  unsigned long b:5;
  unsigned long c:3;
} t;

unsigned long
test_arch1 ()
{
  return t.b;
}

__attribute__ ((target("arch=mips32r2")))
unsigned long
test_arch2 ()
{
  return t.b;
}
