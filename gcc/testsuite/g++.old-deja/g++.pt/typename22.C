// Build don't link:
// Origin: Mark Mitchell <mark@codesourcery.com>

// Implicit typename is removed, so this gives errors.

template <class T>
struct S {
  template <class U>
  struct I { 
    typedef U X;

    X f();
    X g();
    X h();
  };
};


template <class T>
template <class U>
typename S<T>::I<U>::X S<T>::I<U>::f() {} // ERROR - expected

template <class T>
template <class U>
typename S<T>::template I<U>::X S<T>::I<U>::g() {} // ERROR - expected

template <class T>
template <class U>
typename S<T>::template I<U>::X S<T>::template I<U>::h() {} // ok
