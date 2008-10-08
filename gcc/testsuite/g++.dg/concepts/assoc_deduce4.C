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

  result_type call(F&);
}

template<Callable0 F> F::result_type generate(F f) { return f(); }

int zero() { return 0; }

concept_map Callable0<int(*)()> { // { dg-error "ambiguous deduction" }
  int operator()(int (*&f)()) { return f(); } // { dg-error "int" }

  double call(int (*&f)()) { return f(); } // { dg-error "double" }
};

struct Pi {
  double operator()() { return 3.14159; }
};

concept_map Callable0<Pi> { // { dg-error "ambiguous deduction" }

  double operator()(Pi& pi) { return pi(); } // { dg-error "double" }

  float call(Pi& pi) { return pi(); } // { dg-error "float" }
};

void test()
{
  int x = generate(zero); // { dg-error "no matching function" }
  double pi = generate(Pi()); // { dg-error "no matching function" }
}
