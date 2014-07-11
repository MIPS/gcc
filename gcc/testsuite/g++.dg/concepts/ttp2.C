// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C() { return __is_class(T); }

template<typename T>
  concept bool D() { return C<T>() and __is_class(T); }

template<template<typename Q> requires C<Q>() class X>
  struct S { };

template<typename A> requires true struct T0 { };
template<typename A> requires D<A>() struct T1 { };

S<T2> x3; // { dg-error "mismatch" }
S<T3> x4; // { dg-error "mismatch" }

int main() { }
