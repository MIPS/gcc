// { dg-do run }
#include <concepts>

concept Negatable<typename T>
{
  T operator-(const T&);
};

concept RefinesNegatable<typename T> : Negatable<T>
{
};

template<typename T> struct X {};
template<typename T> struct Y {};

template<typename T>
concept_map RefinesNegatable<X<T> >
{
  X<T> operator-(const X<T>& x) { return x; }
};

template<typename T>
concept_map Negatable<Y<T> >
{
  Y<T> operator-(const Y<T>& y) { return y; }
};

template<Negatable T> requires std::CopyConstructible<T>
T negate(const T& x) { return -x; }


int main()
{
  negate(X<int>());
  negate(Y<float>());
  return 0;
}
