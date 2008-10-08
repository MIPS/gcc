auto concept ConstructibleFrom<typename T, typename U> {
  T::T(const U&);
};

template <class T, class U> 
requires ConstructibleFrom<T, U>, ConstructibleFrom<T, T>
void make(const U& u) {
  T x = u;
}

template <class T> 
struct many_constructors {
  template <class U> many_constructors(U);
};

void test_make() {
  make<many_constructors<float> >(1);
}
