#include <concepts>

concept JustAName<typename T>
{
};

template<typename X> requires JustAName<X>
void bar(const X& x)
{
  X y(x); // { dg-error "inaccessible" }
}

template<typename T> requires JustAName<T>, std::CopyConstructible<T>
T bar(const T& t)
{
  return t;
}
