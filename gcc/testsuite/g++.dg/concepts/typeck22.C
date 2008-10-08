// Contributed by Rolf Bonderer
// { dg-options "-std=c++0x" }
#include <concepts>
#include <utility>

auto concept VectorSpace<typename VS>
 {
   typename vector_type = VS::vector_type;
   vector_type new_vector(const VS& vs);
   requires std::CopyConstructible<vector_type>;

   typename scalar_type = VS::scalar_type;
   requires std::DefaultConstructible<scalar_type>;

   typename magnitude_type;
   requires std::DefaultConstructible<magnitude_type>;
 };

template <VectorSpace VS>
std::pair< VectorSpace<VS>::scalar_type, VectorSpace<VS>::vector_type >
//void //->no problems if I choose void as return value and comment out the return statement
function(const VS& vs) {

 typedef VectorSpace<VS>::vector_type vector_type;
 typedef VectorSpace<VS>::scalar_type scalar_type;
 typedef VectorSpace<VS>::magnitude_type magnitude_type;

 vector_type vec1 = new_vector(vs);
 scalar_type lambda;
 magnitude_type residual;

 return std::make_pair(lambda,vec1);
}
