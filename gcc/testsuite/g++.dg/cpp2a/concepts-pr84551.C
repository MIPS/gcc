// PR c++/84551
// { dg-do compile { target c++2a } }
// { dg-additional-options "-g -O" }

template<typename> concept C = true;

template<template<typename T> requires C<T> class> struct A {};

template<typename> requires true struct B {};

A<B> a;
