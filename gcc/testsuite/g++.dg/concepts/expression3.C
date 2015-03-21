// { dg-options "-std=c++1z" }

template<typename T>
concept bool C1()
{
  return requires (T t) { t.f(); };
}

template<typename T>
concept bool C2()
{
  return requires { typename T::type; };
}

class S
{
  using type = int;
  void f() { }
} s;

int main()
{
  static_assert(!C1<S>(), "");
  static_assert(!C2<S>(), "");
}

