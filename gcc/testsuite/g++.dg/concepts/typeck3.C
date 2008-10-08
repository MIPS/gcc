auto concept Swappable<typename T1, typename T2 = T1>
{
  void swap(T1&, T2&);
};

concept NothrowSwappable<typename U1, typename U2 = U1> : Swappable<U1, U2>
{
  // Swap cannot throw
  void swap(U1&, U2&) throw ();
};

template<typename T> requires Swappable<T, T>
void do_swap(T& x, T& y)
{
  swap(x, y);
}

template<typename T> requires NothrowSwappable<T, T>
void do_nothrow_swap(T& x, T& y) // { dg-error "candidates are" }
{
  swap(x, y);
}

template<typename X, typename Y> requires Swappable<X, X>, Swappable<Y, Y>
void multi_swap(X& x, Y& y)
{
  swap(x, x);
  swap(y, y);
  do_swap(x, x);
  do_swap(y, y);
  do_nothrow_swap(x, x); // { dg-error "no matching function" }
  // { dg-error "no concept map for requirement" "" { target *-*-* } 31 }
  // { dg-error "you may need to add the requirement" "" { target *-*-* } 31 }
}

template<typename T> requires NothrowSwappable<T, T>
void do_swap2(T& x, T& y)
{
  do_swap(x, y);
}
