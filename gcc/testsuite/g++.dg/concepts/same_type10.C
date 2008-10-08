#include <concepts>

concept Iter<typename X> {
  typename reference;
  typename value_type;
  reference operator*(X);
};

template<Iter X>
requires std::SameType<X::reference, X::value_type&>
void blah(X& x) {
  X::value_type& z = *x;
}
