// { dg-options "-std=c++0x" }
// Contributed by Karl Meerbergen
namespace std {
  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}

concept ConstFoo<typename X> {
  typename const_pointer = X::const_pointer ;
  const_pointer X::ptr() const ; // Does not compile
  const_pointer operator() ( X const& ) ; // Compiles
} ;

concept Foo<typename X>
: ConstFoo<X>
{} ;

struct foo {
  typedef int*        pointer ;
  typedef int const*  const_pointer ;

  const_pointer ptr() const {
    return data_ ;
  }

  const_pointer operator() () const {
    return data_ ;
  }

  pointer data_ ;
} ;


concept_map Foo<foo> {} ; // Does not compile
//concept_map ConstFoo<foo> {} ; // Compiles

template <ConstFoo X>
void ff( X& x ) {}

int main () {

  foo f ;
  ff( f ) ;

  return 0 ;
}
