// PR c++/67138
// { dg-options "-std=c++2a" }

template <class T>
concept _Auto = true;

template <_Auto T>
struct test {};

template <_Auto T>
  requires requires (T t) { t + t; }
struct test<T> {};
