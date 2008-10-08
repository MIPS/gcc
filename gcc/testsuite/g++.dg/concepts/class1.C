// { dg-do compile }
// { dg-options "-std=c++0x" }
#include <concepts>

auto concept VectorSpace<typename VS> {

  typename scalar_type = VS::scalar_type;
  //...
};

template<VectorSpace VS>
class test {

public:
  typedef VectorSpace<VS>::scalar_type scalar_type;

private:
  scalar_type i;

};
