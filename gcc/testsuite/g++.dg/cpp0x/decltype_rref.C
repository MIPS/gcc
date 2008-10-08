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

class A {};

A&& foo() {}
const A&& fooc() {}

CHECK_DECLTYPE(decltype(foo()), A&&);
CHECK_DECLTYPE(decltype(fooc()), const A&&);

A a1;
A&& a2 = a1;

CHECK_DECLTYPE(decltype(a2), A&&);
CHECK_DECLTYPE(decltype((a2)), A&);

void bar(A&& t) {
  CHECK_DECLTYPE(decltype(t), A&&);
}


// Rvalue reference-collapse rules

int i;
typedef int&  LRI;
typedef int&& RRI;
LRI& r1 = i;            // r1 has the type int&
const LRI& r2 = i;      // r2 has the type int&
const LRI&& r3 = i;     // r3 has the type int&
RRI& r4 = i;            // r4 has the type int&
RRI&& r5 = i;           // r5 has the type int&&

CHECK_DECLTYPE(decltype(r1), int&);
CHECK_DECLTYPE(decltype(r2), int&);
CHECK_DECLTYPE(decltype(r3), int&);
CHECK_DECLTYPE(decltype(r4), int&);
CHECK_DECLTYPE(decltype(r5), int&&);


// rvalue reference members

struct B {
  int&& m;
  B();
  B(int&& i) : m(i) {}
};

B b;
CHECK_DECLTYPE(decltype(b.m), int&&);
