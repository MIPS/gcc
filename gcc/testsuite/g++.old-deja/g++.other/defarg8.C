// Build don't link:
// Special g++ options: -pedantic-errors -ansi -w

// Copyright (C) 2000, 2001 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 7 Jan 2001 <nathan@codesourcery.com>

struct AA
{
  static int Foo (int = Baz ());
  static int Baz (int = Foo ());
};

int main ()
{
  AA::Foo ();
  AA::Foo (1);
  AA::Baz ();
  AA::Baz (1);
  
  return 0;
}
