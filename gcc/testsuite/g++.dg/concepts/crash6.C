#include<concepts>

using namespace std;

auto concept Polygon<typename Poly> {
  typename length_type;
  length_type::length_type(int); // { dg-bogus "internal" "" { xfail *-*-* } }
  requires Convertible<length_type,int>;
  requires Convertible<int, length_type>;
  length_type side_length( const Poly&, int );
  int num_sides( const Poly& );
}

template<typename Poly>
requires Polygon<Poly> && Convertible<Polygon<Poly>::length_type,int> && Convertible<int, Polygon<Poly>::length_type>
Polygon<Poly>::length_type circumference( const Poly& p ) {
  Polygon<Poly>::length_type result = 0;
  for( int i = 0; i < num_sides(p); ++i ) {
    result = result + side_length(p,i);
  }
  return result;
}
