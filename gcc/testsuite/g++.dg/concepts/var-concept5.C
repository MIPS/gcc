// { dg-options "-std=c++1z" }

template<typename Deriv, typename Base>
concept bool DerivedFrom = __is_base_of(Base, Deriv);

template<typename T>
constexpr bool is_DerivedFrom( )  { return false; }

template<DerivedFrom T> // { dg-error "wrong number" }
constexpr bool is_DerivedFrom( )  { return true; } 

