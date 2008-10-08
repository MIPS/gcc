#include <concepts>

template<typename T, typename Pred> requires std::BinaryPredicate<Pred, T, T>
  bool negate(const T& t, Pred pred)
  {
    return !pred(t, t);
  }

bool same(const int& x, const int& y)
{
  return x == y;
}

void f()
{
  negate(5, &same);
}
