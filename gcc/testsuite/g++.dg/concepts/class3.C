concept None<typename T> { };

template<None T>
struct foo {
  void bar(T& x, T& y) {
    x + y; // { dg-error "no match" }
  }
};
