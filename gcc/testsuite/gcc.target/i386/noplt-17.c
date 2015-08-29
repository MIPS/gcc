/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -mno-popcnt -fno-pic -fno-plt=__popcountsi2,__popcountdi2" } */

int
foo (unsigned int i)
{
  return __builtin_popcount (i);
}

/* { dg-final { scan-assembler "call\[ \t\]*.__popcountdi2@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "call\[ \t\]*.__popcountsi2@GOT" { target { ia32 && r_386_got32x } } } } */
