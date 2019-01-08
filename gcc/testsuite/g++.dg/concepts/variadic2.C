// { dg-options "-std=c++2a" }

template <class T> concept Copyable = requires (T t) { T(t); };
template <class T> concept Constructable = requires { T(); };
template <class T> concept Both = Copyable<T> && Constructable<T>;

template <Copyable... Ts>
constexpr int f(Ts...) { return 0; } // #1

template <Both... Ts>
constexpr int f(Ts...) { return 1; }     // #2

int main()
{
  static_assert(f(42) == 1);
}
