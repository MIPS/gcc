// { dg-do compile }
// { dg-options "-std=c++2a" }

// Test deduction requirements.

// req12.C

template <typename T, typename U>
concept SameAs = __is_same_as(T, U);

template <typename T>
concept C1 = requires(T t) {
  { t } -> SameAs<T>;
};

template <typename T>
  requires C1<T>
constexpr bool f1() { return true; }

static_assert(f1<char>());
static_assert(f1<int>());
static_assert(f1<double>());


template <typename T>
concept C2 = requires(T t) {
  { t } -> SameAs<T&>;
};

template <typename T>
  requires C2<T>
constexpr bool f2() { return true; }

static_assert(f2<int>()); // { dg-error "cannot call|does not satisfy placeholder constraints" }
