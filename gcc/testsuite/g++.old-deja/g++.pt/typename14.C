// Build don't link:
// Special g++ Options:

// Implicit typename is removed, so this gives errors

template <class T>
struct B {
  typedef T X;
};

template <class T>
struct S : public B<T>
{
  struct I {
    void f(X x);	// ERROR - expected
  };
};
