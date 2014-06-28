// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

template<typename T>
  requires C<T>()
    void f(T x) { }

// Check call sites in templates when the args are non-dependent.
template<typename T>
  void h(T x) {
    f(0); // { dg-error "matching" }
  }

int main() {
  f(0); // { dg-error "matching" }
}
