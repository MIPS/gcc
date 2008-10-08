// { dg-do compile }
namespace std {
  auto concept CopyConstructible<typename T> {
    T::T(const T&);
    T::~T();
  };
}

template<typename T, typename U>
class pair
{
 private:
  T first;
  U second;
};

auto concept Assignable<typename T, typename U = T> {
  typename result_type;
  result_type T::operator=(U);
}

template<typename T> requires !Assignable<T> void foo(T&);

void bar(pair<const int, int>& p)
{
  foo(p);
}
