// { dg-options "-std=c++98 -pedantic-errors" }
template<typename... Args> class tuple; // { dg-error "variadic templates" }
