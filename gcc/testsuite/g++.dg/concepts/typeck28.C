// { dg-options "-std=c++0x" }
// Contributed by Jeremy Siek
#include <concepts>

concept Fooable<typename T> {
  T foo(T);
};

template<Fooable T>
requires std::CopyConstructible<T>
class bar {
public:
  T yaba(T x) {
    return foo(x);
  }
};

concept_map Fooable<int> {
  int foo(int x) { return x; }
}

int main() {
  bar<int> b;
  return b.yaba(0);
}
