// Build don't link:

// Copyright (C) 2000, 2001 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 7 Jan 2001 <nathan@codesourcery.com>

struct A
{
  static int Foo (int = Baz ());	// ERROR - circularity
  static int Baz (int = Foo ());
};

struct Test
{
  Test (void * = 0);
  void set (const Test &arg = Test ());
};

struct B
{
  static int Bar (int = Foo (1));
  static int Foo (int = Baz ());	// ERROR - circularity
  static int Baz (int = Foo (1));
};

int main ()
{
  Test t;
  t.set ();
  t.set (t);
  B::Bar ();
  B::Bar (1);
  B::Baz ();
  B::Baz (1);
  B::Foo ();
  B::Foo (1);
  
  return 0;
}
