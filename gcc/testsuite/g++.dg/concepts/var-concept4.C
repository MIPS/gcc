// { dg-options "-std=c++1z" }

template<typename T, typename U>
concept bool Same = __is_same_as(T, U);

template<typename T0, typename T1, typename T2, typename... T3toN>
concept bool Same<T0, T1, T2, T3toN...> = true; // { dg-error "wrong number|does not match" }
