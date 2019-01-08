// { dg-options "-std=c++2a" }

template<int N, class T>
  concept P = true;

template<template<typename> class X, class T>
  concept Q = true;

template<P<int> N> void f() { }
template<Q<int> X> void g() { }

template<typename> struct S { };

int main() {
  f<0>();
  g<S>();
}
