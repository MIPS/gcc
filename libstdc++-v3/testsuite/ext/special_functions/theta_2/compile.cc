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

// theta_2

#include <cmath>

void
test01()
{
  float nuf = 1.5F;
  double nud = 1.5;
  long double nul = 1.5L;

  float xf = 0.5F;
  double xd = 0.5;
  long double xl = 0.5L;

  __gnu_cxx::theta_2(nuf, xf);
  __gnu_cxx::theta_2f(nuf, xf);
  __gnu_cxx::theta_2(nud, xd);
  __gnu_cxx::theta_2(nul, xl);
  __gnu_cxx::theta_2l(nul, xl);

  __gnu_cxx::theta_2(nuf, xd);
  __gnu_cxx::theta_2f(nuf, xf);
  __gnu_cxx::theta_2(nud, xl);
  __gnu_cxx::theta_2(nul, xf);
  __gnu_cxx::theta_2l(nul, xl);

  return;
}

