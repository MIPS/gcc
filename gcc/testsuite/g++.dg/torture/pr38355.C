// { dg-do run }
// { dg-options "-fwhole-program -fipa-struct-reorg" }
template<int> struct A
{
  char c;
  void foo(int);
  void bar(int i) { foo(i+1); }
};

template<int> struct B : virtual A<0> {};

template<int T> inline void baz(B<T>& b, int i)
{
  if (i) b.bar(0);
}

extern template class A<0>;
extern template void baz(B<0>&, int);

int main()
{
  B<0> b;
  baz(b, 0);
  return 0;
}
