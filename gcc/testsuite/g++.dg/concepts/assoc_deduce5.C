namespace std {
  auto concept CopyConstructible<typename T> {
    T::T(const T&);
    T::~T();
  };
}

concept Callable0<typename F>
{
  std::CopyConstructible result_type;
  result_type operator()(F&);
}

template<Callable0 F> F::result_type generate(F f) { return f(); }

int zero() { return 0; }

concept_map Callable0<int (*)()> { };

struct Pi {
  double operator()() { return 3.14159; }
};

concept_map Callable0<Pi> { };

void test()
{
  int x = generate(zero);
  double pi = generate(Pi());
}
