// { dg-do link }
// { dg-bogus "already defined" "concepts break the ABI" { xfail *-*-* } 0 }
concept A<typename T> {};
concept B<typename T> {};

template<typename T> requires A<T> 
int foo(T&, int x, int y) { return x+y; }

template<typename T> requires B<T> 
int foo(T&, int x, int y) { return 2*x + y % 365; }

template<typename T> requires A<T> int foo_as_A(T& x) { return foo(x, 5, 17); }
template<typename T> requires B<T> int foo_as_B(T& x) { return foo(x, 5, 17); }

concept_map A<int> { };
concept_map B<int> { };

extern "C" {
  void abort(void);
}

int main()
{
  int x = 5;
  if (foo_as_A(x) != 22) abort();
  if (foo_as_B(x) != 27) abort();
}
