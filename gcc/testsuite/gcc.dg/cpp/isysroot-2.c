/* APPLE LOCAL SDK 3886137 */
/* Patch is waiting FSF review. */
/* { dg-options "-isysroot ${srcdir}/gcc.dg/cpp" } */
/* { dg-do compile } */

#include <Carbon/Carbon.h>
int main()
{
  /* Special Carbon.h supplies function foo.  */
  foo();
  return 0;
}
