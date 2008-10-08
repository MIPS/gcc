namespace std {

auto concept CopyConstructible<typename T>
{
  T::T(const T&);
  T::~T();
};

}

auto concept Convertible<typename T, typename U>
{
  operator U(const T&);
};

auto concept Callable1<typename F, typename X> {
  typename result_type = F::result_type;
  result_type operator()(F, X);
};

template<typename R, typename P1, typename A1>
  requires Convertible<A1, P1> && 
           std::CopyConstructible<P1> && 
           std::CopyConstructible<R>
  concept_map Callable1<R (*)(P1), A1> { typedef R result_type; };
