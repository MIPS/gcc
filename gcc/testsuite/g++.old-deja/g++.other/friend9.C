// Build don't link:

// Copyright (C) 2000, 2001 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 24 Nov 2000 <nathan@codesourcery.com>

// Bug 853: We reported the wrong line no for a friend access violation

// Since DR 209, friend declaration access is not checked.

// Actually Issue 209 was resolved as NAD.

class F
{
  class Internal; // ERROR - private
};

class C
{
  friend class F::Internal; // ERROR - F::internal is inaccessible
  public:
  typedef enum { A, B } e;

  C ();
  ~C();

  void m();
};
