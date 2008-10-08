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
  T x;
  T y = x; // { dg-error "inaccessible" }
  y = x; // { dg-error "inaccessible" "" { xfail *-*-* } }
}
