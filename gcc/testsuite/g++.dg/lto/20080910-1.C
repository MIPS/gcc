// FIXME lto.  We should really use 'compile' instead of 'assemble', but
// -S confuses cc1plus.
// { dg-do assemble }
struct Foo { Foo(int); }; void func() { new Foo(0); }
