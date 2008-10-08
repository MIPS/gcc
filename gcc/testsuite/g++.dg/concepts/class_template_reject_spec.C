// { dg-do compile }
#include <concepts>
template<typename T, unsigned N> class array {};


template<typename T> 
struct spec_vs_requires
{
  enum { value = 0 };
};

template<typename T, unsigned N> requires std::True<(N > 0)>
struct spec_vs_requires<array<T, N> >
{
  enum { value = 1 };
};


void foo()
{
  int get_spec[spec_vs_requires<array<int, 1> >::value == 1? 1 : 0];
  int get_primary[spec_vs_requires<array<int, 0> >::value == 0? 1 : 0];
}
