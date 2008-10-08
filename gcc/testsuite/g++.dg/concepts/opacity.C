concept Nothing<typename T> { };

template<typename T, Nothing U>
void foo1(const T& t, const U& u)
{
  t + u; // { dg-error "no match for" }
}

late_check template<typename T, typename U> requires Nothing<U>
void foo2(const T& t, const U& u)
{
  t + u;
}
