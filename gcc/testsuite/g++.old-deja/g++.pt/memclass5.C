template <class T> struct A {
  template <class U> struct B {
    template <class V> static void f () { }
    void g () { }
  };
};

template <class T, class U>
void f ()
{
  typedef typename A<T>::template B<U> X;
  X::template f<T> ();
  typename A<T>::template B<U> b;
  b.X::~X();
}

template <class T> struct C: public A<T>::B<T> {};
  
int main ()
{
  f<int, char>();
}
