#include <concepts>

template<typename T, typename U>
struct pair
{
  pair(const T&, const U&);
};

template<typename X, std::CopyConstructible Y>
pair<X, Y> make_pair(const X& x, const Y& y) { return pair<X, Y>(x, y); }

template pair<int*, float*> make_pair(int* const&, float* const&);
