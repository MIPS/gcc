// -*- C++ -*-
// Testing utilities for the rvalue reference.
//
// Copyright (C) 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.
//
// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

#ifndef _GLIBCXX_TESTSUITE_RVALREF_H
#define _GLIBCXX_TESTSUITE_RVALREF_H 1

#include <testsuite_hooks.h>

namespace __gnu_test
{

  class rvalstruct
  {
    bool
    operator=(const rvalstruct&);

// Normally we don't define a copy constructor, as any use of it would
// show an inefficency. In some cases we know it will be aliased away
// by the compiler, but it still insists it is defined, so we provide
// a way of making it public but not giving a body, so any usage would
// instead fail at link-time.
#ifdef _GLIBCXX_TESTSUITE_ALLOW_RVALREF_ALIASING
  public:
    rvalstruct(const rvalstruct&);
#else
    rvalstruct(const rvalstruct&);

  public:
#endif
    int val;
    bool valid;

    rvalstruct() : valid(false)
    { }

    rvalstruct(int inval) : val(inval), valid(true)
    { }
    
    rvalstruct&
    operator=(int newval)
    { 
      VERIFY(valid == false);
      val = newval; 
      valid = true;
    }

    rvalstruct(__gnu_cxx::__rvalref<rvalstruct> in)
    { 
      VERIFY(in.__ref.valid == true);
      val = in.__ref.val;
      in.__ref.valid = false;
      valid = true;
    }

    rvalstruct&
    operator=(__gnu_cxx::__rvalref<rvalstruct> in)
    { 
      VERIFY(in.__ref.valid == true);
      val = in.__ref.val;
      in.__ref.valid = false;
      valid = true;
      return *this;
    }
  };

  bool 
  operator==(const rvalstruct& lhs, 
 	     const rvalstruct& rhs)
  { return lhs.val == rhs.val; }

  bool
  operator<(const rvalstruct& lhs, 
	    const rvalstruct& rhs)
  { return lhs.val < rhs.val; }

  void
  swap(rvalstruct& lhs, rvalstruct& rhs)
  {  
    VERIFY(lhs.valid && rhs.valid);
    int temp = lhs.val;
    lhs.val = rhs.val;
    rhs.val = temp;
  }

}; // namespace __gnu_test

namespace __gnu_cxx 
{
  template<>
    struct __is_moveable<__gnu_test::rvalstruct>
    { static const bool value = true; };
}

#endif // _GLIBCXX_TESTSUITE_TR1_H
