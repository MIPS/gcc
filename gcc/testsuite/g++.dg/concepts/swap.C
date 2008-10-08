// { dg-do run }
extern "C" {
  void abort(void);
}

auto concept Assignable<typename T, typename U = T>
{
  T& operator=(T&, const U&);
};

auto concept CopyConstructible<typename T>
{
  T::T(const T&);
};

template<typename T>
requires Assignable<T>, CopyConstructible<T>
inline void
swap(T& a, T& b)
{
  T tmp = a;
  a = b;
  b = tmp;
}

int main()
{
  int x = 17;
  int y = 42;
  swap(x, y);
  if (x != 42 || y != 17)
    abort();
  return 0;
}
