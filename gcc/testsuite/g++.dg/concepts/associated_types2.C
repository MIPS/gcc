#include <concepts>
concept BiggerType<typename T>
{
  typename bigger_type;
};

concept_map BiggerType<int>
{
  typedef long bigger_type;
};

concept_map BiggerType<float>
{
  typedef double bigger_type;
};

template<typename T, typename BiggerT> 
  requires std::SameType<BiggerType<T>::bigger_type, BiggerT>
  void foo() {}

void check()
{
  foo<int, long>();
  foo<float, double>();
}
