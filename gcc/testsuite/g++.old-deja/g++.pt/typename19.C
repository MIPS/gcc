// Special g++ Options:
// Origin: Mark Mitchell <mark@codesourcery.com>

// Implicit typename is removed, so this gives errors now.

template <class T>
struct O {
  typedef char X;
};

template <class T>
struct S {
  typedef double X;

  template <class U>
  struct I : public O<U> {
    static X x; // lookup finds S<T>::X
    static X y;
  };
};

template <class T>
template <class U>
typename S<T>::X S<T>::I<U>::x;	// ERROR - expected

template <class T>
template <class U>
typename S<T>::X S<T>::template I<U>::y;	// ok

int main()
{
  return sizeof (S<int>::I<double>::x) == 1;
}
