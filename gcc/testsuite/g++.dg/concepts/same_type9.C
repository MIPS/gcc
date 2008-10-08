#include <concepts>

concept Fooable<typename F> {
  typename type;
};

concept Barable<typename F> {
  typename type;
};

template<Fooable T>
concept_map Barable<T> {
  typedef Fooable<T>::type type;
};

template<Barable B1, Barable B2>
requires std::SameType<Barable<B1>::type, Barable<B2>::type>
void bar(B1&, B2&) { } // { dg-bogus "candidates" "" { xfail *-*-* } }

template<Fooable F1, Fooable F2>
requires std::SameType<Fooable<F1>::type, Fooable<F2>::type>
void foo(F1& f1, F2& f2) {
  bar(f1, f2); // { dg-bogus "no matching|same-type" "" { xfail *-*-* } }
}
