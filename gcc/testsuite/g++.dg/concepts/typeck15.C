concept ConvertibleToIntegral<typename T>
{
  operator long long(const T&);
};

template<typename T> requires ConvertibleToIntegral<T>
bool less(const T& x, int y)
{
  return x < y;
}
