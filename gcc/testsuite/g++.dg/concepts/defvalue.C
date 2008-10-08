#include <concepts>
using namespace std;

template<CopyConstructible T> T identity(const T& x = T()) { return x; }

template<CopyConstructible T>
struct foo
{
  foo(const T& x = T()) { }
};
