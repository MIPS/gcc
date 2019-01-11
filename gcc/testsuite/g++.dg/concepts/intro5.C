// { dg-do compile { target c++17 } }
// { dg-options "-fconcepts" }

template<typename T, typename U = int>
  concept bool C()
  {
     return sizeof(U) == sizeof(int);
  }

C{A} void f1() {} // { dg-error "all template parameters" }

