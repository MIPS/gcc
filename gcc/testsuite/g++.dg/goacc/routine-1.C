/* Test valid use of the routine directive.  */

namespace N
{
  extern void foo1();
  extern void foo2();
#pragma acc routine (foo1) seq
#pragma acc routine seq
  void foo3()
  {
  }
}
#pragma acc routine (N::foo2) seq
