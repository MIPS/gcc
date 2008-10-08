// { dg-options "-std=c++0x" }
#include <concepts>

namespace boost {

  class seed_seq { };

concept UniformRandomNumberGenerator< class U >
  : ::std::Callable0<U>
{
  typename result_t = U::result_type;
  requires ::std::UnsignedIntegralLike< result_t >;

  #ifdef NOTYET
  result_t const  U::min;
  result_t const  U::max;
  #endif  // NOTYET

  #ifdef OOPS
  axiom Bounded(U)  {
    U::min <= U();
    U() <= U::max;
  }
  #endif  // OOPS
};  // UniformRandomNumberGenerator<>


concept RandomNumberEngine< class E >
  : UniformRandomNumberGenerator<E>
  , ::std::DefaultConstructible<E>
  , ::std::CopyConstructible<E>
  , ::std::CopyAssignable<E>
  , ::std::EqualityComparable<E>
  //  , ::std::Streamable<E>  // TODO
{
  E::E( UniformRandomNumberGenerator<E>::result_type );
  E::E( ::boost::seed_seq & );

  void  E::seed();
  void  E::seed( UniformRandomNumberGenerator<E>::result_type );
  void  E::seed( ::boost::seed_seq & );

  void  E::discard( unsigned long long );

};  // RandomNumberEngine<>

}
