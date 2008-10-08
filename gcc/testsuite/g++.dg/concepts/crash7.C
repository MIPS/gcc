#include <concepts>
#include <cassert>

template< std::IntegralLike X >
void foo( X const& x ) {
  assert( x==x ) ;
}

int main() {
  foo( 5 ) ;
  return 0 ;
}
