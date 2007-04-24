// { dg-do "compile" }
// { dg-options "-std=gnu++0x" }

template<typename T, typename U> 
struct is_same 
{
  static const bool value = false;
};

template<typename T>
struct is_same<T, T>
{
  static const bool value = true;
};

#define CHECK_DECLTYPE(DECLTYPE,RESULT) \
  static_assert(is_same< DECLTYPE , RESULT >::value, #RESULT)

template<typename F> F create_a();

template<typename F, typename T1, typename T2>
bool 
check(F f, T1 a1, T2 a2, 
      decltype(create_a<F&>()(create_a<T1&>(), create_a<T2&>())) result)
{
  return result == f(a1, a2);
}

struct X {};
bool operator==(X, X) { return true; }

struct MakeX {
  X operator()(int&, float&) { return X(); }
};

void check()
{
  check(MakeX(), 17, 3.14f, X());
}
