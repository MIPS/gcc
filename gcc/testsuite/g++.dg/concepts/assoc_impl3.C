// { dg-options "-std=c++0x" }
// { dg-do "compile" }
auto concept Foo<typename T> {
  bool foo(T);
  bool bar(T t) { return foo(t); } // { dg-error "not declared" }
  bool wibble(T) { return false; }
  bool wonka(T) { return false; }
}

template<typename T> struct X { };

template<typename T> bool foo(X<T>) { return true; }
template<typename T> bool wonka(X<T>) { return true; }

extern "C" {
  void abort();
}

template<typename T> concept_map Foo<X<T>> { }

template<Foo T>
void check(T t)
{
  if (!foo(t)) abort();
  if (!bar(t)) abort();
  if (wibble(t)) abort();
  if (!wonka(t)) abort();
}

int main()
{
  check(X<int>());
}
