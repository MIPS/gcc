// { dg-do compile }

// Copyright (C) 2001 Free Software Foundation, Inc.
// Contributed by Nathan Sidwell 24 Sept 2001 <nathan@codesourcery.com>

// Applying attributes could clobber existing nodes in the type system

struct A
{
  static void (*Ptr1) (int);
  static void (__attribute__ ((__stdcall__)) *Ptr) (int);
};

void (*A::Ptr1) (int) = 0;

void (__attribute__ ((__stdcall__)) *A::Ptr) (int) = 0;
