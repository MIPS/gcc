// { dg-options "-std=c++98 -pedantic" }
template<typename... Args> class tuple; // { dg-warning "variadic templates" }
