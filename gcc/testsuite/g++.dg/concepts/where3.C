#include <concepts>

template<std::CopyConstructible T>
struct list {
  requires std::LessThanComparable<T> T silly_min(const T& x, const T& y) 
  { 
    return x < y? x : y; 
  }
};

struct X {};

int f()
{
  list<int> lint;
  lint.silly_min(5, 17);

  list<X> lX;
  lX.silly_min(X(), X()); // { dg-error "has no member named" }
}
