concept FooMethod<typename T>
{
  int T::foo(int);
};

template<typename T>
  requires FooMethod<T>
  int foo(T& t)
  {
    return t.foo(17);
  }

template<typename T>
  requires FooMethod<T>
  int bar(T& t)
  {
    return t.bar(17); // { dg-error "has no member named" }
  }
