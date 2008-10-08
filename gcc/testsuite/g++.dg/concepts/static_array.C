#include<concepts>

template <std::DefaultConstructible DC>
void foo (DC val)
{
  static DC vals[8];
}

int main ()
{
  foo(3);
}
