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

// radpoly

#include <cmath>

void
test01()
{
  unsigned int n = 2;
  unsigned int m = 1;
  float rhof = 0.25F;
  double rhod = 0.25;
  long double rhol = 0.25L;

  __gnu_cxx::radpoly(n, m, rhod);
  __gnu_cxx::radpolyf(n, m, rhof);
  __gnu_cxx::radpoly(n, m, rhod);
  __gnu_cxx::radpoly(n, m, rhod);
  __gnu_cxx::radpolyl(n, m, rhol);

  return;
}

