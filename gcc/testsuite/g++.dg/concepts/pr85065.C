// { dg-do compile { target c++14 } }
// { dg-additional-options "-fconcepts" }

template<int> concept bool C = true;

C c = 0;  // { dg-error "does not designate a type|does not name a type" }
