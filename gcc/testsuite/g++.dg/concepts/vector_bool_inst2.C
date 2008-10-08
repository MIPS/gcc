#include <vector>

template<std::CopyConstructible T>
void specialization_hole(T x)
{
  std::vector<T> vec(1, x);
  T& ref = vec.front(); // { dg-error "invalid initialization of non-const reference of type 'bool&' from a temporary of type 'std::_Bit_reference'" }
}

template void specialization_hole<bool>(bool);
