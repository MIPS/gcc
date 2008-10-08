concept Nothing<typename> { };
template<typename T> requires Nothing<T>
  typename T::value_type bar(T&); // { dg-error "does not name a type" }
// { dg-error "invalid type in declaration" "" { target *-*-* } 3 }
