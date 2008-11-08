// { dg-do compile }
struct Foo { Foo(int); }; void func() { new Foo(0); }
