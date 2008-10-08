// { dg-do compile  }

template <class T = int>
struct A { // { dg-error "is private" }
  const T x;
  A() : x(0) { } A(T x) : x(x) { }
}; 

template <class B>
void func ()
{
  B y; 
  y = B();  // { dg-error "synthesized" }
}

int main (void) { func< A<> >(); } // { dg-error "instantiated from here" }

// { dg-error "within this context" "" { target *-*-* } 13 }
// { dg-error "non-static const member" "" { target *-*-* } 4 }

