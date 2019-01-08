// PR c++/84551
// { dg-options "-g -O -std=c++2a" }

template<typename> concept C = true;

template<template<typename T> requires C<T> class> struct A {};

template<typename> requires true struct B {};

A<B> a;
