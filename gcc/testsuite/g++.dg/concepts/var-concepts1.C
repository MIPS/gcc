// { dg-do run }
// { dg-options "-std=c++1z" }

template<typename T>
  concept bool C1 = __is_class(T);

void f1(C1, C1);

struct S1 {};

int main ()
{
  f1(S1(), S1());
  return 0;
}
 
template<typename U>
  requires C1<U>
  void f1(U, U)
  {
  }
