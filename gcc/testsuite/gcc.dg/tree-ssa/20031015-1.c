/* With tree-ssa, gcc.dg/20000724-1.c fails because we miss a VUSE
   of x in the asm statement.  */
/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ssa1-vops" } */
struct s {
  int a;
};
int
main(void)
{
  struct s x = { 0 };
  asm volatile ("call ___checkme" : : "c" (&x) : "memory");
  return 0;
}
/* { dg-final { scan-tree-dump-times "VUSE " 1 "ssa1"} } */
