/* { dg-do compile } */
/* { dg-options "-g -fdump-early-debug-stats" } */

int main()
{
  typedef int myint;
  myint myintvar;
  int yesblockvar=5;
}
/* { dg-final { scan-assembler "DUMPED EARLY: main" } } */
/* { dg-final { scan-assembler "DUMPED EARLY: myintvar" } } */
/* { dg-final { scan-assembler "DUMPED EARLY: yesblockvar" } } */
