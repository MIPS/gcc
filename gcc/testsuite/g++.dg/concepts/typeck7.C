namespace std {
  concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept Silly<typename T>
{
  typename product_type;
  product_type operator*(const T&, const T&);

  T operator+(const T&, product_type);
};

template<typename T> requires Silly<T>
bool foo(const T& x, const T& y, const T& z)
{
  return x + y * z; // { dg-error "cannot convert" }
}
