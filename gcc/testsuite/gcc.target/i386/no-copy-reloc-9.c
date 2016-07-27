/* { dg-do compile { target *-*-linux* } } */
/* { dg-options "-O2 -fpie -mno-copy-reloc" } */

extern int glob_a;

int foo ()
{
  return glob_a;
}

/* { dg-final { scan-assembler "glob_a@GOTPCREL" { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "glob_a@GOT\\(" { target { ia32 } } } } */
