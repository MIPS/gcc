// { dg-options "-std=c++1y" }

#include <cassert>

template<typename T>
  concept bool C() { return __is_class(T); }

struct S { } s;

int called;

// Basic terse notation
void f(auto x) { called = 1; }
void g(C x) { called = 2; }

// Overloading generic functions
void h(auto x) { called = 1; }
void h(C x) { called = 2; }

void p(auto x);
void p(C x);

struct S1 {
  void f1(auto x) { called = 1; }
  void f2(C x) { called = 2; }

  void f3(auto x) { called = 1; }
  void f3(C x) { called = 2; }
};

template<C T>
  struct S2 {
    void f1(auto x) { called = 1; }
    void f2(C x) { called = 2; }

    void f3(auto x) { called = 1; }
    void f3(C x) { called = 2; }

    void h1(auto x);
    void h2(C x);

    void h3(auto x);
    void h3(C x);

    template<C U>
      void g1(T t, U u) { called = 1; }

    template<C U>
      void g2(T t, U u);
  };


int main() {
  f(0); assert(called == 1);
  g(s); assert(called == 2);

  h(0); assert(called == 1);
  h(s); assert(called == 2);

  S1 s1;
  s1.f1(0); assert(called == 1);
  s1.f2(s); assert(called == 2);
  // s1.f2(0); // Error

  s1.f3(0); assert(called == 1);
  s1.f3(s); assert(called == 2);

  S2<S> s2;
  s2.f1(0); assert(called == 1);
  s2.f2(s); assert(called == 2);
  // s2.f2(0); // Error

  s2.f3(0); assert(called == 1);
  s2.f3(s); assert(called == 2);

  s2.h1(0); assert(called == 1);
  s2.h2(s); assert(called == 2);
  // s2.h2(0); // Error

  s2.h3(0); assert(called == 1);
  s2.h3(s); assert(called == 2);

  s2.g1(s, s); assert(called == 1); 
  // s2.g(s, 0); // Error
  // s2.g(0, s); // Error

  s2.g2(s, s); assert(called == 2);
  // s2.g(s, 0); // Error
}

void p(auto x) { called = 1; }
void p(C x) { called = 2; }

template<C T>
  void S2<T>::h1(auto x) { called = 1; }

template<C T>
  void S2<T>::h2(C x) { called = 2; }

template<C T>
  void S2<T>::h3(auto x) { called = 1; }

template<C T>
  void S2<T>::h3(C x) { called = 2; }

template<C T>
  template<C U>
    void S2<T>::g2(T t, U u) { called = 2; }
