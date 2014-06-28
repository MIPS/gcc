// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

struct S { } s;

template<typename T>
  requires C<T>()
    void f(T x) { }

// Calls to constrained functions are valid.
template<typename T>
  void g(T x) { f(x); }

template<typename T>
  void h(T x) {
    f(s);
  }

int main() {
  f(s);
  g(s);
}
