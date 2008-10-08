// { dg-do compile }
// { dg-options "-std=c++0x" }
// Contributed by Rolf Bonderer
#include<concepts>

auto concept Vector<typename V, typename scalar_type = typename V::value_type>
{
 requires std::HasMultiply<scalar_type, V>; //operation "scalar*vector" valid
 requires std::HasPlus<V, std::HasMultiply<scalar_type, V>::result_type>;
 //...
};
