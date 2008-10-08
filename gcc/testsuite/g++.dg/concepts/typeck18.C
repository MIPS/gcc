namespace std {
  concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept Convertible<typename T, typename U>
{
  operator U(const T&);
};

concept CopyConstructible<typename T>
{
  T::T(const T&);
};

concept InputIterator<typename X> : CopyConstructible<X>
{
  typename value_type;
  typename reference;

  requires Convertible<reference, value_type>;

  reference operator*(const X&);
};

concept ForwardIterator<typename X> : InputIterator<X>
{
  requires Convertible<reference, const value_type&>;
};

template<ForwardIterator Iter> requires CopyConstructible<Iter::value_type>
void foo(const Iter& first)
{
  Iter::value_type v = *first;
}
