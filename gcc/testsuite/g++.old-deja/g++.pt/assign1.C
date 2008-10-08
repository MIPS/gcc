// { dg-do compile  }
// Origin: Mark Mitchell <mark@codesourcery.com>

template <class T>
struct S { // { dg-error "is private" }
  S();
  T t;
};

void f()
{
  S<const int> s;
  s = s; // { dg-error "synthesized" }
}

// { dg-error "within this context" "" { target *-*-* } 13 }
// { dg-error "non-static const member" "" { target *-*-* } 5 }
