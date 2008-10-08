concept LessThanComparable<typename T> {
  bool operator<(const T&, const T&);
}

template<typename T>
requires LessThanComparable<T>
const T& max(const T& x, const T& y) {
  return x > y? x : y; // { dg-error "no match" }
}

late_check template<typename T>
requires LessThanComparable<T>
const T& max_unsafe(const T& x, const T& y) {
  return x > y? x : y;
}
