// { dg-do compile }
concept C<typename T> { };

template<typename T>
  requires C<T>, !C<T> // { dg-error "tautology" }
  void foo(T&);
