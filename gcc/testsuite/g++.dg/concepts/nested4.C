// { dg-do compile }
namespace std {
  concept CopyConstructible<typename T> {
    T::T(const T&);
    T::~T();
  };
}

concept Integral<typename T> { 
  T::T();
};

concept Iterator<typename Iter> {
  typename difference_type;
  requires Integral<difference_type>;
};

template<Iterator Iter> Iter::difference_type foo(const Iter&)
{
  Iter::difference_type x;
}
