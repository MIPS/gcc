#include <concepts>

// -----------------------------------------------------------------------------

concept equality_comparable<typename T1, typename T2>
{
  bool operator== (T1 const&, T2 const&);
};

concept_map equality_comparable<int, int> {};

// -----------------------------------------------------------------------------

concept predicate2<typename F, typename A0, typename A1>
{
  bool operator()(F&, A0, A1);
};

// -----------------------------------------------------------------------------

template <typename T1, typename T2 = T1>
requires equality_comparable<T1, T2>
struct equal_to
{
  bool operator()(T1 const& v1, T2 const& v2) const
  {
    return v1 == v2;
  }
};

template <typename T1, typename T2>
requires equality_comparable<T1, T2>
concept_map predicate2<equal_to<T1, T2>, T1, T2>
{
};

// -----------------------------------------------------------------------------

template <typename T1, typename T2>
requires equality_comparable<T1, T2>
bool compare(T1 const& v1, T2 const& v2)
{
  equal_to<T1, T2> eq;
  return eq(v1, v2);
}

// -----------------------------------------------------------------------------
