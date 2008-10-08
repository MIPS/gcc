concept UnaryOperation<typename Op, typename T> {
  void operator()(Op&, T);
}

template<typename T, UnaryOperation<T> Op>
void apply(Op& op, const T& t)
{
  op(t);
}
