auto concept Swappable<typename T1, typename T2 = T1>
{
  void swap(T1&, T2&);
};

template<typename T> requires Swappable<T, T>
void do_swap(T& x, T& y)
{
  swap(x, y);
}

template<typename T, typename U> requires Swappable<T, T>
void no_swap(T& x, U& y)
{
  swap(x, y); // { dg-error "invalid initialization" }
  // { dg-error "in passing argument 2" "" { target *-*-* } 3 }
}

concept Nothing<typename> { };
template<Nothing T>
void no_swap2(T& x, T& y)
{
  swap(x, y); // { dg-error "no function named" }
}

template<typename T, typename U> requires Swappable<T, T>, Swappable<U, U>
void multi_swap(T& x, U& y)
{
  swap(x, x);
  swap(y, y);
}

template<typename T> int swap(T&, T&);

template<Nothing T>
void ext_swap(T& x, T& y)
{
  swap(x, y);
}

template<typename T, typename U> requires Swappable<T, T>
void hide_ext_swap(T& x, U& y)
{
  swap(x, x);
  swap(y, y); // { dg-error "invalid initialization" }
  // { dg-error "in passing argument 1" "" { target *-*-* } 3 }
}

template<typename T, typename U> requires Swappable<T, T>
void using_ext_swap(T& x, U& y)
{
  using ::swap;
  swap(y, y);
  swap(x, x) + 17;
}

void f()
{
  int i = 17;
  float j = 3.14f;
  using_ext_swap(i, j);
}


