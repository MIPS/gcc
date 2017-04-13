// { dg-require-c-std "" }
// { dg-add-options ieee }
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

  float alphaf = std::numeric_limits<float>::quiet_NaN();
  double alphad = std::numeric_limits<double>::quiet_NaN();
  long double alphal = std::numeric_limits<long double>::quiet_NaN();

  float xf = 0.55F;
  double xd = 0.55;
  long double xl = 0.55L;

  float a = __gnu_cxx::gegenbauer(n, alphaf, xf);
  float b = __gnu_cxx::gegenbauer(n, alphaf, xf);
  double c = __gnu_cxx::gegenbauer(n, alphad, xd);
  long double d = __gnu_cxx::gegenbauer(n, alphal, xl);
  long double e = __gnu_cxx::gegenbauer(n, alphal, xl);

  bool test [[gnu::unused]] = true;
  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

void
test02()
{
  unsigned int n = 4;

  float alphaf = 2.0F;
  double alphad = 2.0;
  long double alphal = 2.0L;

  float xf = std::numeric_limits<float>::quiet_NaN();
  double xd = std::numeric_limits<double>::quiet_NaN();
  long double xl = std::numeric_limits<long double>::quiet_NaN();

  float a = __gnu_cxx::gegenbauer(n, alphaf, xf);
  float b = __gnu_cxx::gegenbauer(n, alphaf, xf);
  double c = __gnu_cxx::gegenbauer(n, alphad, xd);
  long double d = __gnu_cxx::gegenbauer(n, alphal, xl);
  long double e = __gnu_cxx::gegenbauer(n, alphal, xl);

  bool test [[gnu::unused]] = true;
  VERIFY(std::isnan(a));
  VERIFY(std::isnan(b));
  VERIFY(std::isnan(c));
  VERIFY(std::isnan(d));
  VERIFY(std::isnan(e));

  return;
}

int
main()
{
  test01();
  test02();
  return 0;
}
