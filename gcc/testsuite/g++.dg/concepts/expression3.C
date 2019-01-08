<<<<<<< HEAD
// { dg-do compile { target c++17 } }
// { dg-options "-fconcepts" }
=======
// needs port; no clear match
// { dg-options "-std=c++17 -fconcepts" }
>>>>>>> 594131fbad3... move ported tests; note more issues and needs port after fixes

template<typename T>
concept bool C()
{
  return requires (T& t) { t.~T(); };
}

class S1
{
  ~S1() { }
};

class S2
{
  ~S2() = delete;
};

int main()
{
  static_assert(C<S1>(), ""); // { dg-error "failed" }
  static_assert(C<S2>(), ""); // { dg-error "failed" }
}
