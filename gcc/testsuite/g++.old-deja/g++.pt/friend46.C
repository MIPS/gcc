// Build don't link:
// Copyright (C) 2000, 2001 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 17 Nov 2000 <nathan@codesourcery.com>

// bug 43. When instantiating, we'd lookup in the wrong scope.

namespace X {
  template <class T> class P;
  
  template <class T> void operator- (const P<T>&);
  
  template <class T>
  struct V
  {
    V (const T&);
  
    void operator- ();
    friend void ::X::operator-<> (const P<T>& a);
  };
}

int main()
{
  X::V<double> b(1.0);

  return 0;
}
