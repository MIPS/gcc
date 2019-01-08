// { dg-do compile }
// { dg-options "-std=c++2a" }

// A poor mans Integral concept.
template<typename T>
concept Integral = __is_same_as(T, int);

template<int N>
concept Nonnegative = N >= 0;

template<typename... Args>
concept UnaryPack = (sizeof...(Args) == 1);

template<typename... Args>
  requires Integral<Args...> // { dg-error "pack expansion argument" }
void f1();

template<typename... Args>
  requires Integral<Args>... // { dg-error "parameter packs not expanded|expected unqualified-id" }
void f2();

template<typename... Args>
  requires (Integral<Args> && ...)
void f3() { }

template<Integral... Args>
void f4() { }

// FIXME: This syntax is likely to be made invalid.
template<Nonnegative... Args>
void f5() { }

template<UnaryPack Args> // { dg-error "variadic constraint" }
void f6() { }

// FIXME: This syntax is likely to be made invalid.
template<UnaryPack... Args>
void f7() { }

void driver()
{
  f1<int, int>(); // { dg-error "cannot call function" }
  f3<int, int>();
  f3<int, void>(); // { dg-error "cannot call function" }
  f4<int, int>();
  f4<int, void>(); // { dg-error "cannot call function" }
  f5<0, 1, 2, 3>();
  f5<1, 0, -1>(); // { dg-error "cannot call function" }
  f7<int>();
  f7<int, int>(); // { dg-error "cannot call function" }
}
