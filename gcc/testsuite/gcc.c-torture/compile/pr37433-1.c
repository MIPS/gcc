/* MIPS compressed ISAs require the LSB of an address to indicate which
   ISA mode to use.  This test cannot do that and raises an assembler
   warning (binutils 2.29 onwards) of a branch to a different ISA.  */
/* { dg-skip-if "" { mips_compressed } } */

void regex_subst(void)
{
  const void *subst = "";
  (*(void (*)(int))subst) (0);
}

void foobar (void)
{
  int x;
  (*(void (*)(void))&x) ();
}
