// Build don't run:
// Special g++ Options:

// Implicit typename is removed, so this gives errors

struct B {
  typedef int I;
};

template <class T>
struct D1 : public B {
};

template <class T>
struct D2 : public D1<T> {
  I i;	// ERROR - expected
};

template <>
struct D1<int> {
  typedef double I;
};

template <class T>
void f(T);
template <>
void f(double) {}

int main()
{
  D2<int> d2i;
  f(d2i.i);	// ERROR - no member
}
