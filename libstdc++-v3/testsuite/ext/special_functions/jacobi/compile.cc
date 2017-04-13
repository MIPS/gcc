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

// jacobi

#include <cmath>

void
test01()
{
  unsigned int n = 2;
  float alphaf = 1.5F, betaf = 3.25F, xf = 0.5F;
  double alphad = 1.5, betad = 3.25, xd = 0.5;
  long double alphal = 1.5L, betal = 3.25L, xl = 0.5L;

  __gnu_cxx::jacobi(n, alphal, betad, xf);
  __gnu_cxx::jacobif(n, alphaf, betaf, xf);
  __gnu_cxx::jacobi(n, alphad, betad, xd);
  __gnu_cxx::jacobi(n, alphaf, betad, xl);
  __gnu_cxx::jacobil(n, alphal, betal, xl);

  return;
}

