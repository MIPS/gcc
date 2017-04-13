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

// ellint_rg

#include <cmath>

void
test01()
{
  float xf = 2.0F, yf = 3.0F, zf = 0.5F;
  double xd = 2.0, yd = 3.0, zd = 0.5;
  long double xl = 2.0L, yl = 3.0L, zl = 0.5L;

  __gnu_cxx::ellint_rg(xf, yf, zf);
  __gnu_cxx::ellint_rgf(xf, yf, zf);
  __gnu_cxx::ellint_rg(xd, yd, zd);
  __gnu_cxx::ellint_rg(xl, yl, zl);
  __gnu_cxx::ellint_rgl(xl, yl, zl);

  return;
}

