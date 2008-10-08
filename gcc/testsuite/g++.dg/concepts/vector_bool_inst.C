#include <vector>

template<std::CopyConstructible T>
void specialization_hole(T x)
{
  std::vector<T> vec(1, x);
  T& ref = vec.front();
}

template void specialization_hole<int>(int);
