template<typename T> struct foo { static int bar(); };

template<> struct foo<int> { static void bar(); };

auto concept Nothing<typename T> { };

template<Nothing U> int g()
{
  return foo<U>::bar(); // { dg-error "void value not ignored" }
}

int main()
{
  return g<int>();
}
