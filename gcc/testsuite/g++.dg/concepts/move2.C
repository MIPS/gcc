concept Moveable<typename T> {
  T::T(T&&);
  T& T::operator=(T&&);
}

concept DefaultConstructible<typename T> {
  T::T();
}

template<Moveable T> requires DefaultConstructible<T>
void f()
{
  T x(T());
  T y = T(); // { dg-bogus "inaccessible" "" { xfail *-*-* } }
}
