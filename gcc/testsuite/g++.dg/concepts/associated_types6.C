concept Superior<typename T>
{
  typename value_type;
  void set(const value_type& x);
};

concept Refined<typename T> : Superior<T>
{
};

template<typename T>
struct X {};

template<typename T>
concept_map Refined<X<T> >
{
  typedef T value_type;

  void set(const T&) {}
};

