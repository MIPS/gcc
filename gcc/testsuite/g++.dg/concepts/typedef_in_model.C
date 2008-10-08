concept Fooable<typename T>
{
  typename type;
  void foo(const type&);
};

template<typename T>
struct X
{
};

template<typename T>
concept_map Fooable<X<T> >
{
  typedef T type;
  void foo(const T& t) { }
};

template<typename T>
struct Y
{
};

template<typename T>
concept_map Fooable<Y<T> >
{
  typedef T type;
  void foo(const type& t) { }
};
