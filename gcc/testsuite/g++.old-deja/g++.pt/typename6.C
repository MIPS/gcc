// We don't try to make implicit typename handle this case.

// Implicit type name is removed, so this test gives other errors

// Build don't link:
// Special g++ Options:

template <class T>
struct A
{
  typedef T A_Type;
};


template <class U>
struct B : public A<U>
{
  A_Type Func();		// ERROR - expected
};

template <class U>
A<U>::A_Type B<U>::Func()      // ERROR - expected
{
}
