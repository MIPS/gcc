concept InitWithInt<typename T>
{
  T::T(int);
};

concept LessThanComparable<typename T>
{
  bool operator<(const T&, const T&);
};

template<typename T>
requires InitWithInt<T>, LessThanComparable<T>
int foo(const T& t, int n)
{
  if (t < n)
    return 17;
  else
    return 0;
}
