// { dg-options "-std=c++1z" }

// Check argument deduction constraints.
// TODO: We shoul have more of these...

template<typename T>
concept bool C1 = false;

template<typename T, typename U>
concept bool C2 = false;


template<typename T>
concept bool D1()
{
  return requires (T t) { { t } -> C1; };
}

template<typename T>
concept bool D2()
{
  return requires (T t) { { t } -> C2<int>; };
}

void f1(D1) { }
void f2(D2) { }

int main()
{
  f1(0); // { dg-error "cannot call" }
  f2(0); // { dg-error "cannot call" }
}
