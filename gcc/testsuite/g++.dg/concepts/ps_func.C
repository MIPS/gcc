namespace std {

  auto concept CopyConstructible<typename T>
  {
    T::T(const T&);
    T::~T();
  };
}


concept Fooable<typename T>
{
  bool foo(T);
};

class X {};
bool foo(X) { return true; }

class Y { static bool foo(Y) { return false; } };

class Z {};

concept_map Fooable<X> {};

void f()
{
  X x;
  Fooable<X>::foo(x);
}

concept_map Fooable<Y> {}; // { dg-error "conversion from" }
// { dg-error "unsatisfied requirement" "" { target *-*-* } 31 }
// { dg-error "in" "" { target *-*-* } 31 }

template<typename T> bool foo(T) { return false; }

concept_map Fooable<Z> {};

template<typename T>
concept_map Fooable<T*> { };

void g()
{
  Fooable<int*>::foo(0);
}

