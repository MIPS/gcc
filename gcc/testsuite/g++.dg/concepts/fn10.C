// { dg-do compile }
// { dg-options "-std=c++1z" }

// Test that constraint satisfaction checks work even when
// processing template declarations.

namespace std
{
  struct ostream { };
  ostream cout;
}

template <typename T>
  concept bool Float()
  {
    return __is_same_as( T, float );
  }

template <typename T>
  constexpr decltype(auto) project( T t )
  {
    return t;
  }

template <typename T>
  concept bool Concept()
  {
    return requires( T t ) {
      requires Float<decltype( project(t) )>();
    };
  }

template <Concept E, Concept F>
  constexpr decltype(auto) operator<<( E&& e, F&& f ) {}

template <Concept T>
  void foo( T t )
  {
    // Try to resolve operator<< from within a template context but
    // with non-dependent arguments. We need to ensure that template
    // processing is turned off whenever checking for satisfaction.
    std::cout << "OK"; // { dg-error "no match" }
  }

int main()
{
  foo( 12.5f );
  return 0;
}
