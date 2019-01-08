// { dg-do compile { target c++14 } }
// { dg-additional-options "-std=c++2a -fconcepts-ts" }

template<typename> concept bool C = true;

C{} void foo();  // { dg-error "expected identifier" }
