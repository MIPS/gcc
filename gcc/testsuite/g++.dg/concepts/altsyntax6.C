#include <concepts>

concept UnaryOperation<typename Op, typename T> {
  typename result_type;
  result_type operator()(Op&, T);
}

template<typename T, UnaryOperation<T> Op>
requires std::CopyConstructible<Op::result_type>
Op::result_type apply(Op& op, const T& t)
{
  return op(t);
}
