// Build don't link:
// Special g++ Options:

// Implicit typename is removed, so this now should give errors

template <class T>
struct A
{
  typedef T A_Type;
};


template <class U>
struct B : public A<U>
{
};


template <class U>
struct C : public B<U>
{
  void Func(A_Type);  // ERROR - expected
};
