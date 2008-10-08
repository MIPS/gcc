concept Fooable<typename T>
{
  typename type;
  void foo(const type&);
};

concept RefinesFooable<typename T> : Fooable<T>
{
};

template<typename T>
struct X
{
};

template<typename T>
concept_map RefinesFooable<X<T> >
{
  typedef T type;
  void foo(const T& t) { }
};

template<typename T>
struct Y
{
};

template<typename T>
concept_map RefinesFooable<Y<T> >
{
  typedef T type;
  void foo(const type& t) { }
};
