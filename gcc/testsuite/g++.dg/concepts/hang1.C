// { dg-options "-std=c++0x" }
concept A<typename T> { };
concept B<typename T> : A<T> { };
concept C<typename T> : A<T> { };
concept D<typename T> : B<T>,C<T> { };

template<typename T> requires A<T> void foo(T&) { }
template<typename T> requires B<T> void foo(T&) { }
template<typename T> requires C<T> void foo(T&) { }
template<typename T> requires D<T> void foo(T&) { }

template<typename T> requires A<T>
void bar(T& x) { foo(x); }

struct X { };
concept_map D<X> { };

int main() {
  X x;
  bar(x);
}
