concept CopyConstructible<typename T>
{
  T::T(const T&);
};

template<typename T, typename U>
struct pair
{
  pair(const T& first, const U& second) : first(first), second(second) { }

  T first;
  U second;
};

template<typename T, typename U>
  requires CopyConstructible<T>, CopyConstructible<U>
void make_pair_simple(const T& t, const U& u)
{
  pair<T, U>(t, u);
  pair<T, U>(t, t); // { dg-error "no matching function for call" }
  // { dg-error "candidates are" "" { target *-*-* } 9 }
  // { dg-error "pair" "" { target *-*-* } 8 }
}
