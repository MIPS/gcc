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

// zernike

#include <cmath>

void
test01()
{
  unsigned int n = 2;
  int m = 1;
  float rhof = 0.25F, phif = 0.5F;
  double rhod = 0.25, phid = 0.5;
  long double rhol = 0.25L, phil = 0.5L;

  __gnu_cxx::zernike(n, m, rhod, phif);
  __gnu_cxx::zernikef(n, m, rhof, phif);
  __gnu_cxx::zernike(n, m, rhod, phid);
  __gnu_cxx::zernike(n, m, rhod, phil);
  __gnu_cxx::zernikel(n, m, rhol, phil);

  return;
}

