// { dg-do compile }
class Foo { virtual void f(); };
class Bar:public Foo { };
void func() { Bar(); }
