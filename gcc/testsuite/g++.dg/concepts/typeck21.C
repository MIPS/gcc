concept LessThanComparable<typename T, typename U = T> {
  bool operator<(const T&, const U&);
  bool operator<(const U&, const T&);
}

template<LessThanComparable _Tp>
  inline const _Tp&
  min(const _Tp& __a, const _Tp& __b)
  {
    if (__b < __a)
      return __b;
    return __a;
  }
