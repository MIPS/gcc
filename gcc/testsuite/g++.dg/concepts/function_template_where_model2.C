// { dg-do compile }
#include <concepts>

concept PointerLike<typename T>
{
};

concept SmartPointerLike<typename T> : PointerLike<T>
{

};

concept_map PointerLike<int*> {};

template<typename T> concept_map PointerLike<const T*> {};

concept_map SmartPointerLike<double*> {};


template<typename T> concept_map SmartPointerLike<volatile T*> {};

template<typename T> requires PointerLike<T> void foo(T) {}

void bar()
{
  int* i = 0;
  float* f = 0;
  const float* cf = 0;
  double* d = 0;
  volatile double* vd = 0;
  foo(i);
  foo(f); // { dg-error "no matching" }
  // { dg-error "candidates are" "" { target *-*-* } 22 }
  // { dg-error "no concept map for requirement" "" { target *-*-* } 32 }
  // { dg-error "syntactically matches" "" {target *-*-* } 4 }
  // { dg-error "if the concept semantics are met" "" {target *-*-* } 32 }
  // { dg-error ".*concept_map PointerLike.*" "" {target *-*-* } 0 }
  foo(cf);
  foo(d);
  foo(vd);
}
