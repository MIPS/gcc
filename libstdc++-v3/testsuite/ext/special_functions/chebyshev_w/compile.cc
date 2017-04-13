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

// chebyshev_w

#include <cmath>

void
test01()
{
  float xf = 0.75F;
  double xd = 0.75;
  long double xl = 0.75L;

  unsigned int n = 5;

  __gnu_cxx::chebyshev_w(n, xf);
  __gnu_cxx::chebyshev_wf(n, xf);
  __gnu_cxx::chebyshev_w(n, xd);
  __gnu_cxx::chebyshev_w(n, xl);
  __gnu_cxx::chebyshev_wl(n, xl);

  return;
}

