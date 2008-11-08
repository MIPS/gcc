// { dg-do compile }
// { dg-options "-flto-single" }

struct Foo
{
 virtual void func() = 0;
};

struct Bar
{
 Foo *field;
 void func2();
};

struct Baz
{
 Bar &bar();
 Baz();
};

struct Zonk
{
 virtual ~Zonk() {
 }
 virtual void func3() = 0;
};

void Mumble(Zonk *) {
}

extern "C"
{
 void __attribute__ ((nothrow)) __cxa_pure_virtual() {
   Baz().bar().func2();
 }
}
