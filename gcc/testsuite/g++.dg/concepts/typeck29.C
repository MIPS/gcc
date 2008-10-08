// { dg-do "compile" }
// { dg-options "-std=c++0x" }

// Contributed by Karl Meerbergen
#include <concepts>

namespace glas {

concept VectorExpression<typename T> { 
  std::EqualityComparable size_type;
  
  size_type T::size() const;
};

concept VectorCollection<typename T> { 
  std::EqualityComparable size_type;

  size_type T::size();
};

template <VectorExpression E, VectorCollection C>
  requires std::SameType<typename E::size_type, typename C::size_type>
bool foo( C& c, E const& e ) {
  return c.size() == e.size() ;
}

struct bar2 {
  template <VectorExpression E, VectorCollection C>
    requires std::SameType<typename E::size_type, typename C::size_type>
  bool operator() ( C& c, E const& e ) const {
    return c.size() == e.size() ;
  }
};

template <VectorExpression E, VectorCollection C>
  requires std::SameType<typename E::size_type, typename C::size_type>
struct bar {
  bool operator() ( C& c, E const& e ) const {
    return c.size() == e.size() ;
  }
};

}

struct ybar {
  template <class T>
    requires std::EqualityComparable<T>
  bool operator() ( T const& t1, T const& t2 ) {
    return t1 == t2 ;
  }
} ;


template <class T>
  requires std::EqualityComparable<T>
struct ybar2 {
  bool operator() ( T const& t1, T const& t2 ) {
    return t1 == t2 ;
  }
} ;

