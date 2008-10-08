// { dg-options "-std=c++0x" }
// Contributed by Christopher Eltschka
concept Foo<typename T> {};
concept Bar<typename T>: Foo<T> {};

template<Foo T> struct X
{
  requires Bar<T>
   void foo() const {}
};

template<Bar T> void f(X<T>& x)
{
  x.foo();
}
