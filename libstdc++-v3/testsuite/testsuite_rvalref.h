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

namespace __gnu_test
{

  struct rvalstruct
  {
    static int empty_construct;
    static int copy_construct;
    static int move_construct;
    static int copy_assign;
    static int move_assign;
    
    static void
    zero_counters()
    {
      empty_construct = copy_construct = move_construct = 0;
      copy_assign = move_assign = 0;
    }

    int val;

    rvalstruct() : val(0)
    { ++empty_construct;}
      
    rvalstruct(int in) : val(in)
    { }

    rvalstruct(const rvalstruct& in)
    {
      val=in.val;
      ++copy_construct;
    }

    rvalstruct(__gnu_cxx::__rvalref<rvalstruct> in)
    {
      val=in.__ref.val;
      ++move_construct;
    }

    rvalstruct&
    operator=(const rvalstruct& in)
    { 
      ++copy_assign;
      val = in.val;
      return *this;
    }

    rvalstruct&
    operator=(__gnu_cxx::__rvalref<rvalstruct> in)
    { 
      ++move_assign;
      val = in.__ref.val;
      return *this;
    }
  };

bool 
operator==(const rvalstruct& lhs, const rvalstruct& rhs)
{ return lhs.val == rhs.val; }

bool
operator<(const rvalstruct& lhs, const rvalstruct& rhs)
{ return lhs.val < rhs.val; }
  
int rvalstruct::empty_construct = 0;
int rvalstruct::copy_construct = 0;
int rvalstruct::move_construct = 0;
int rvalstruct::copy_assign = 0;
int rvalstruct::move_assign = 0;

}; // namespace __gnu_test

namespace __gnu_cxx 
{
  template<>
    struct __is_moveable<__gnu_test::rvalstruct>
    { static const bool value = true; };
}

#endif // _GLIBCXX_TESTSUITE_TR1_H
