// needs port; no clear match
// TODO: ICE on gimplify 16?
// { dg-do run }
// { dg-options "-std=c++2a" }

#include <cassert>
#include <iostream>

template<typename T>
  concept C1 = __is_class(T);

template<typename T>
  concept C3 = requires (T a) { ++a; };

int main() {
  if (C1<int>) assert(false);
  if (!C3<int>) assert(false);
}
