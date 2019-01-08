// PR c++/66844
// { dg-options "-std=c++2a" }

template <class T, class U>
concept Same = __is_same_as(T, U);

template <class T>
concept C = requires (T t) {
    requires Same<decltype(t),void>;
  };

template <typename T>
  requires C<T>
constexpr bool is_c() { return true; }

static_assert(is_c<void>(), "");

int main() {}
