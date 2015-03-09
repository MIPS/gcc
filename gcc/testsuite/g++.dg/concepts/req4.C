// { dg-options "-std=c++1z" }

struct fool {
  constexpr fool operator&&(fool) const { return {}; }
  constexpr fool operator||(fool) const { return {}; }
};

template<typename T> constexpr fool p1() { return {}; }
template<typename T> constexpr fool p2() { return {}; }

template<typename T>
  concept bool C() { return p1<T>() && p2<T>(); } 

template<C T> void f(T x) { } // { dg-error "user-defined operator" }

int main() {
  // FIXME: We should be diagnosing the failed lookup
  // of operator&&, not the fact that neither have the
  // right type (which is true, but not the best error).
  f(0); // { dg-error "not|bool" }
}
