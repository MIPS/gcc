// { dg-do run }
#include <concepts>

extern "C" {
  void abort(void);
}

concept Negatable<typename T>
{
  T operator-(const T&);
};

concept RefinesNegatable<typename T> : Negatable<T>
{
};

concept_map RefinesNegatable<int> {};

// Turn negation of longs into squaring of longs
concept_map RefinesNegatable<long>
{
  long operator-(const long& l);
};

long RefinesNegatable<long>::operator-(const long& l)
{
  return l*l;
}

// Turn negation of shorts into the identity function.
concept_map RefinesNegatable<short>
{
  short operator-(const short& s) { return s; }
};

// A type for which one could not instantiate "negate" without a model.
struct X
{
};

concept_map RefinesNegatable<X>
{
  X operator-(const X& x) { return x; }
};

template<Negatable T> requires std::CopyConstructible<T>
T negate(const T& x) { return -x; }


int main()
{
  if (negate(17) != -17) abort();
  if (negate(16l) != 256l) abort();
  if (negate((short)42) != 42) abort();
  negate(X());
  return 0;
}
