// { dg-do compile }

// Copyright (C) 2001 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 23 Sept 2001 <nathan@codesourcery.com>

// Make sure declarator parameters are looked up in the correct scope.

struct A
{
  typedef int Int;

  static const int size = 5;
  
  static void (*Ptr2) (Int);
  static int (*Ary)[size];
  
};

void (*A::Ptr2) (Int) = 0;
int (*A::Ary) [size] = 0;
