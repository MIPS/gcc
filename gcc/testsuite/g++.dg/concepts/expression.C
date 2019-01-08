<<<<<<< HEAD
// { dg-do run { target c++17 } }
// { dg-options "-fconcepts" }
=======
// needs port; no clear match
// TODO: ICE on gimplify 16?
// { dg-do run }
<<<<<<< HEAD
// { dg-options "-std=c++17 -fconcepts" }
>>>>>>> 594131fbad3... move ported tests; note more issues and needs port after fixes
=======
// { dg-options "-std=c++2a" }
>>>>>>> eda685858ca... move more ported tests

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
