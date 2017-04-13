// { dg-do compile }
// { dg-options "-D__STDCPP_WANT_MATH_SPEC_FUNCS__" }

// Copyright (C) 2016 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// double_factorial

#include <cmath>

void
test01()
{
  int n = 50;

  __gnu_cxx::double_factorial<float>(n);
  __gnu_cxx::double_factorialf(n);
  __gnu_cxx::double_factorial<double>(n);
  __gnu_cxx::double_factorial<long double>(n);
  __gnu_cxx::double_factoriall(n);

  return;
}

