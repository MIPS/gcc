/* This test verifies that we mark "l" as a potential target for a
   nonlocal goto.  Failure to do so resulted in "l" being removed
   on the tree-ssa branch, leading to a compilation failure.  */

main()
{
  __label__ l;

  void x()
  {
    goto l;
  }

  x();
  abort();
  return;
l:
  exit(0);
}
