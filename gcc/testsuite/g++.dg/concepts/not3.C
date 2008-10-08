// { dg-do compile}

concept A<typename T> {};
concept B<typename T> {};

template<typename T> requires !A<T> 
  void foo(T&); // { dg-error "candidates are" }

template<typename T> requires A<T> void bar(T& x)
{
  foo(x); // { dg-error "no matching function" }
  // { dg-error "concept map" "" { target *-*-* } 11 }
}

