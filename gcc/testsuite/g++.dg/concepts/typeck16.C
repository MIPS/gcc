namespace std {
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept CopyConstructible<typename T>
{
  T::T(const T&);
};

concept ConvertibleToIntegral<typename T> : CopyConstructible<T>
{
  operator long long(T);
};

template<typename T> requires ConvertibleToIntegral<T>
bool less(const T& x, int y)
{
  return x < y;
}
