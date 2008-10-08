auto concept has_f<typename T> { void f(T); };

template<typename T> struct X
{
  requires has_f<T>
  friend void f(T); // { dg-error "cannot have|if this is" }
  // { dg-warning "declares a non-template" "" { target *-*-* } 6 }
};

X<int> x;
