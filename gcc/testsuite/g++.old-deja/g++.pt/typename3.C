// Build don't link:
// Special g++ Options:

// Implicit typename is removed. This now gives errors.

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
B<U>::A_Type B<U>::Func()	// ERROR - expected
{
}
