// { dg-do run }
// { dg-options "-std=c++1z" }

template<typename T>
  concept bool Class () { return __is_class(T); }

void f1(auto a) requires Class<decltype(a)>() { }
void f2(auto a) requires requires (decltype(a) x) { -x; } { }

struct S { } s;

int main() {
  f1(s);
  f2(0);
}
