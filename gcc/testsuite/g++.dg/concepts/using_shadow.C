// { dg-options "-Wall" }
#include "core_concepts.h"

namespace std {
  auto concept Assignable<typename T> {
    T& operator=(T&, const T&);
  }

  template<typename T> 
  requires CopyConstructible<T>, Assignable<T> 
    void swap(T&, T&);
}

concept Swappable<typename T> {
  void swap(T&, T&);
}

template<Swappable T>
void foo(T&, T&) {
  using std::swap; // { dg-warning "shadows" }
}
