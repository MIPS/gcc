// { dg-options "-std=c++0x" }
// Contributed by Christopher Eltschka
concept C<typename T> {};

concept R<typename T, typename U = typename T::type>
{
  void f(T, U);
};

template<C T> struct X
{
  typedef T type;
};

template<C T> void f(X<T> const&, T const&) {}

template<C T> concept_map R<X<T> > {};
