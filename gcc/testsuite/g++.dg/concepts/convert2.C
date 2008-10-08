#include <concepts>

auto concept ConvertibleToValueType<typename T>
{
  typename value_type;
  operator value_type(const T&);
};

template<typename X>
requires ConvertibleToValueType<X> 
         && std::CopyConstructible<ConvertibleToValueType<X>::value_type>
ConvertibleToValueType<X>::value_type implicit_convert(const X& x)
{
  return x;
}
