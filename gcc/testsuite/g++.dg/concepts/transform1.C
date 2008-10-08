// { dg-do run }
// { dg-options "-std=c++0x" }
#include <algorithm>

struct Double {
  template<typename T>
  T operator()(T x) const { return 2*x; }
};

extern "C" {
  void abort(void);
}

int main()
{
  int array[5] = { 1, 2, 3, 4, 5};
  int result[5];

  std::transform(array, array + 5, result, Double());

  for (int i = 0; i < 5; ++i)
    if (result[i] != 2*array[i])
      abort();

  return 0;
}
