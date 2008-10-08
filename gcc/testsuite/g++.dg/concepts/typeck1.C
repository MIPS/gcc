
  auto concept EqualityComparable<typename T>
  {
    bool operator==(const T& x, const T& y);
  };

template<typename T> requires EqualityComparable<T>
  bool neg(const T& x, const T& y)
  {
    return !(x == y);
  }

template<typename T> requires EqualityComparable<T>
  bool neg_fail(const T& x, const T& y)
  {
    int* z;
    z = x == y; // { dg-error "cannot convert" }
    return !(x == y);
  }
