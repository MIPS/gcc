/* Test that #pragma GCC visibility does not override class member specific settings. */
/* { dg-do compile } */
/* APPLE LOCAL begin mainline 4.0 2005-03-25 */
/* { dg-require-visibility "internal" } */
/* APPLE LOCAL end mainline 4.0 2005-03-25 */
/* { dg-final { scan-assembler "\\.internal.*Foo.methodEv" } } */

#pragma GCC visibility push(hidden)
class __attribute__ ((visibility ("internal"))) Foo
{
  void method();
};
#pragma GCC visibility pop

void Foo::method() { }
