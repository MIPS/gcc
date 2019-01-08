// { dg-options "-std=c++2a" }

// Check argument deduction constraints.
// TODO: We should have more of these...

template<typename T>
concept C1 = sizeof(T) == 0;

template<typename T, typename U>
concept C2 = __is_same_as(T, U);


template<typename T>
concept D1 = requires (T t) { { t } -> C1; };

template<typename T>
concept D2 = requires (T t) { { t } -> C2<void>; };

void f1(auto D1) { }
void f2(auto D2) { }

int main()
{
  f1(0); // { dg-error "cannot call" }
  f2(0); // { dg-error "cannot call" }
}
