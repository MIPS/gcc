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

// ellint_rj

#include <cmath>

void
test01()
{
  float xf = 2.0F, yf = 10.0F, zf = 3.0F, pf = 0.5F;
  double xd = 2.0, yd = 10.0, zd = 3.0, pd = 0.5;
  long double xl = 2.0L, bl = 10.0L, cl = 3.0L, pl = 0.5L;

  __gnu_cxx::ellint_rj(xf, yf, zf, pf);
  __gnu_cxx::ellint_rjf(xf, yf, zf, pf);
  __gnu_cxx::ellint_rj(xd, yd, zd, pd);
  __gnu_cxx::ellint_rj(xl, bl, cl, pl);
  __gnu_cxx::ellint_rjl(xl, bl, cl, pl);

  return;
}

