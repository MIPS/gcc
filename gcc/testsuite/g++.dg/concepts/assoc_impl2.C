// { dg-options "-std=c++0x" }
// { dg-do "run" }
auto concept Foo<typename T> {
  bool foo(T);
  bool bar(T t) { return foo(t); }
  bool wibble(T) { return false; }
  bool wonka(T) { return false; }
}

struct X { };
bool foo(X) { return true; }
bool wonka(X) { return true; }

extern "C" {
  void abort();
}

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
  check(X());
}
