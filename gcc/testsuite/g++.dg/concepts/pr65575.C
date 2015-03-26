// { dg-options "-std=c++1z" }

template<typename T>
concept bool C = false;

auto f() -> int& requires C<int>;

int 
main()
{
  f(); // { dg-error "cannot call" }
}
