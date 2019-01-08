// needs port after issue 48 is resolved; doesn't recognize extra requires as
// more specialized
// PR c++/67138
// { dg-do compile { target c++17 } }
// { dg-options "-fconcepts" }

template <class T>
concept bool _Auto = true;

template <_Auto T>
struct test {};

template <_Auto T>
  requires requires (T t) { t + t; }
struct test<T> {};
