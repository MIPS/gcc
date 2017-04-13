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

// conf_hyperg_lim

#include <cmath>

void
test01()
{
  float cf = 3.0F, xf = 0.5F;
  double cd = 3.0, xd = 0.5;
  long double cl = 3.0L, xl = 0.5L;

  __gnu_cxx::conf_hyperg_lim(cf, xf);
  __gnu_cxx::conf_hyperg_limf(cf, xf);
  __gnu_cxx::conf_hyperg_lim(cd, xd);
  __gnu_cxx::conf_hyperg_lim(cl, xl);
  __gnu_cxx::conf_hyperg_liml(cl, xl);

  return;
}

