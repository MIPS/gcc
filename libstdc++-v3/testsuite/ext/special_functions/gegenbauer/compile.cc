// { dg-do compile }
// { dg-options "-D__STDCPP_WANT_MATH_SPEC_FUNCS__" }
//
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

//  gegenbauer

#include <cmath>
#include <testsuite_hooks.h>

void
test01()
{
  unsigned int n = 4;

  float alphaf = 2.0F, xf = 0.55F;
  double alphad = 2.0, xd = 0.55;
  long double alphal = 2.0L, xl = 0.55L;

  __gnu_cxx::gegenbauer(n, alphaf, xf);
  __gnu_cxx::gegenbauerf(n, alphaf, xf);
  __gnu_cxx::gegenbauer(n, alphad, xd);
  __gnu_cxx::gegenbauer(n, alphal, xl);
  __gnu_cxx::gegenbauerl(n, alphal, xl);

  __gnu_cxx::gegenbauer(n, alphaf, xd);
  __gnu_cxx::gegenbauerf(n, alphaf, xf);
  __gnu_cxx::gegenbauer(n, alphad, xl);
  __gnu_cxx::gegenbauer(n, alphal, xf);
  __gnu_cxx::gegenbauerl(n, alphal, xl);

  return;
}

int
main()
{
  test01();
  return 0;
}
