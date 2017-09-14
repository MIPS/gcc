/* MIPS compressed ISAs require the LSB of an address to indicate which
   ISA mode to use.  This test cannot do that and raises an assembler
   warning (binutils 2.29 onwards) of a branch to a different ISA.  */
/* { dg-skip-if "" { mips_compressed } } */
int regex_subst(void)
{
  const void *subst = "";
  return (*(int (*)(int))subst) (0);
}

int foobar (void)
{
  int x;
  return (*(int (*)(void))&x) ();
}
