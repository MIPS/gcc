// { dg-do compile }
concept C<typename T> {
  typename assoc;
};

template<C T> 
  T::assoc& 
  foo(const T&);

template<C T> 
  typename T::assoc& 
  bar(const T&);

template<typename T> requires C<T> 
  T::assoc&
  wibble(const T&);

template<typename T> requires C<T> 
  typename T::assoc&
  wonka(const T&);

struct X {};
concept_map C<X> { typedef int assoc; }

bool test()
{
  X x;
  int& i1 = foo(x);
  int& i2 = bar(x);
  return i1 == i2;
}
